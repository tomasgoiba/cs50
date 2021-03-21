package edu.harvard.cs50.pokedex;

import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.net.URL;

public class PokemonActivity extends AppCompatActivity {
    private TextView nameTextView;
    private TextView numberTextView;
    private TextView type1TextView;
    private TextView type2TextView;
    private ImageView spriteImageView;
    private TextView descriptionTextView;
    private Button button;

    private boolean caughtPokemon;
    private String url;
    private String name;
    private RequestQueue requestQueue;

    private SharedPreferences mPreferences;
    private SharedPreferences.Editor mEditor;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pokemon);

        requestQueue = Volley.newRequestQueue(getApplicationContext());
        url = getIntent().getStringExtra("url");
        name = getIntent().getStringExtra("name");

        nameTextView = findViewById(R.id.pokemon_name);
        numberTextView = findViewById(R.id.pokemon_number);
        type1TextView = findViewById(R.id.pokemon_type1);
        type2TextView = findViewById(R.id.pokemon_type2);
        spriteImageView = findViewById(R.id.pokemon_sprite);
        descriptionTextView = findViewById(R.id.pokemon_description);
        button = findViewById(R.id.button);

        mPreferences = getPreferences(Context.MODE_PRIVATE);
        mEditor = mPreferences.edit();

        load();
        loadDescription();
        checkSharedPreferences();
    }

    public void load() {
        nameTextView.setText(name);
        type1TextView.setText("");
        type2TextView.setText("");

        JsonObjectRequest request = new JsonObjectRequest(Request.Method.GET, url, null, new Response.Listener<JSONObject>() {
            @Override
            public void onResponse(JSONObject response) {
                try {
                    numberTextView.setText(String.format("#%03d", response.getInt("id")));

                    JSONArray typeEntries = response.getJSONArray("types");
                    for (int i = 0; i < typeEntries.length(); i++) {
                        JSONObject typeEntry = typeEntries.getJSONObject(i);
                        int slot = typeEntry.getInt("slot");
                        String type = typeEntry.getJSONObject("type").getString("name");

                        if (slot == 1) {
                            type1TextView.setText(type);
                        } else if (slot == 2) {
                            type2TextView.setText(type);
                        }
                    }

                    String spriteUrl = response.getJSONObject("sprites").getString("front_default");
                    new DownloadSpriteTask().execute(spriteUrl);

                } catch (JSONException e) {
                    Log.e("cs50", "Pokemon JSON error", e);
                }
            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                Log.e("cs50", "Pokemon details error", error);
            }
        });

        requestQueue.add(request);
    }

    public void loadDescription() {
        descriptionTextView.setText("");

        String url = String.format("https://pokeapi.co/api/v2/pokemon-species/%s/", name.toLowerCase());
        JsonObjectRequest request = new JsonObjectRequest(Request.Method.GET, url, null, new Response.Listener<JSONObject>() {
            @Override
            public void onResponse(JSONObject response) {
                try {
                    JSONArray flavorTextEntries = response.getJSONArray("flavor_text_entries");
                    for (int i = 0; i < flavorTextEntries.length(); i++) {
                        JSONObject flavorTextEntry = flavorTextEntries.getJSONObject(i);
                        if (flavorTextEntry.getJSONObject("language").getString("name").equals("en")) {
                            descriptionTextView.setText(flavorTextEntry.getString("flavor_text"));
                            break;
                        }
                    }
                } catch (JSONException e) {
                    Log.e("cs50", "Pokemon Species JSON error");
                }
            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                Log.e("cs50", "Pokemon Species text error");
            }
        });

        requestQueue.add(request);

    }

    private class DownloadSpriteTask extends AsyncTask<String, Void, Bitmap> {
        @Override
        protected Bitmap doInBackground(String... strings) {
            try {
                URL url = new URL(strings[0]);
                return BitmapFactory.decodeStream(url.openStream());
            }
            catch (IOException e) {
                Log.e("cs50", "Download sprite error", e);
                return null;
            }
        }

        @Override
        protected void onPostExecute(Bitmap bitmap) {
            spriteImageView.setImageBitmap(Bitmap.createScaledBitmap(bitmap, 720, 720, false));
        }
    }

    private void checkSharedPreferences() {
        caughtPokemon = mPreferences.getBoolean(url, false);

        if (caughtPokemon) {
            button.setText("Release");
        } else {
            button.setText("Catch");
        }
    }

    public void toggleCatch(View view) {
        if (caughtPokemon) {
            caughtPokemon = false;
            button.setText("Catch");
        } else {
            caughtPokemon = true;
            button.setText("Release");
        }

        mEditor.putBoolean(url, caughtPokemon);
        mEditor.commit();

    }
}
