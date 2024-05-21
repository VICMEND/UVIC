
#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Created on Wed Feb 8 14:44:33 2023
Based on: https://www.kaggle.com/datasets/arbazmohammad/world-airports-and-airlines-datasets
Sample input: --AIRLINES="airlines.yaml" --AIRPORTS="airports.yaml" --ROUTES="routes.yaml" --QUESTION="q1" --GRAPH_TYPE="bar"
@author: rivera
@author: Victor Mendes
"""
import pandas as pd
import numpy as np
import sys


def argument_processor() -> None:
    """formats command line arguments into pandas compatible values and passes them to data_processor."""
    sort_by: str = sys.argv[1].split('=')[1]
    display_amount: str = int(sys.argv[2].split('=')[1])
    files: str = sys.argv[3].split('=')[1]
    data_processor(sort_by, display_amount, files)


def data_processor(sort_by: str, display_amount: int, files: str) -> None:
    """formats DataFrames according to specified parameters."""
    if ',' in files:                                            # checks if multiple files were passed.
        list_music_df: list[pd.DataFrame] = []
        multi_files: str = files.split(',')
        for file_names in multi_files:
            temp_df: pd.DataFrame = pd.read_csv(file_names)
            list_music_df.append(temp_df)
        music_df: pd.DataFrame = pd.concat(list_music_df, axis=0)
    else:
        music_df: pd.DataFrame = pd.read_csv(files)

    if sort_by == 'popularity':                                    # removes unnecessary columns
        music_df = music_df.drop(['duration_ms', 'explicit', 'danceability', 'energy', 'key', 'loudness', 'mode',
                                 'speechiness', 'acousticness', 'instrumentalness', 'liveness', 'valence', 'tempo', 'genre'], axis=1)
    elif sort_by == 'energy':
        music_df = music_df.drop(['duration_ms', 'explicit', 'popularity', 'danceability', 'key', 'loudness', 'mode',
                                 'speechiness', 'acousticness', 'instrumentalness', 'liveness', 'valence', 'tempo', 'genre'], axis=1)
    elif sort_by == 'danceability':
        music_df = music_df.drop(['duration_ms', 'explicit', 'popularity', 'energy', 'key', 'loudness', 'mode',
                                 'speechiness', 'acousticness', 'instrumentalness', 'liveness', 'valence', 'tempo', 'genre'], axis=1)

    music_df = music_df.sort_values(by=[sort_by, 'song'], ascending=False)
    music_df = music_df.head(display_amount)
    music_df.to_csv('output.csv', index=False)


def main():
    """Main entry point of the program."""
    argument_processor()


if __name__ == '__main__':
    main()
