/***************************************************************************
 *   Copyright (C) 2008-2016 by Andrzej Rybczak                            *
 *   electricityispower@gmail.com                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.              *
 ***************************************************************************/

#ifndef NCMPCPP_LYRICS_FETCHER_H
#define NCMPCPP_LYRICS_FETCHER_H

#include "config.h"

#ifdef HAVE_CURL_CURL_H

#include <memory>
#include <string>

struct LyricsFetcher
{
	typedef std::pair<bool, std::string> Result;
	
	virtual const char *name() const = 0;
	virtual Result fetch(const std::string &artist, const std::string &title);
	
protected:
	virtual const char *urlTemplate() const = 0;
	virtual const char *regex() const = 0;
	
	virtual bool notLyrics(const std::string &) const { return false; }
	virtual void postProcess(std::string &data) const;
	
	std::vector<std::string> getContent(const char *regex, const std::string &data);
	
	static const char msgNotFound[];
};

typedef std::vector<std::unique_ptr<LyricsFetcher>> LyricsFetchers;

std::unique_ptr<LyricsFetcher> toLyricsFetcher(const std::string &s);

/**********************************************************************/

struct LyricwikiFetcher : public LyricsFetcher
{
	virtual const char *name() const override { return "lyricwiki.com"; }
	virtual Result fetch(const std::string &artist, const std::string &title) override;
	
protected:
	virtual const char *urlTemplate() const override { return "http://lyrics.wikia.com/api.php?action=lyrics&fmt=xml&func=getSong&artist=%artist%&song=%title%"; }
	virtual const char *regex() const override { return "<url>(.*?)</url>"; }
	
	virtual bool notLyrics(const std::string &data) const override;
};

/**********************************************************************/

struct GoogleLyricsFetcher : public LyricsFetcher
{
	virtual Result fetch(const std::string &artist, const std::string &title);
	
protected:
	virtual const char *urlTemplate() const { return URL; }
	virtual const char *siteKeyword() const { return name(); }
	
	virtual bool isURLOk(const std::string &url);
	
private:
	const char *URL;
};

struct MetrolyricsFetcher : public GoogleLyricsFetcher
{
	virtual const char *name() const override { return "metrolyrics.com"; }
	
protected:
	virtual const char *regex() const override { return "<div class=\"lyrics-body\">(.*?)</div>"; }
	
	virtual bool isURLOk(const std::string &url) override;
};

struct LyricsmaniaFetcher : public GoogleLyricsFetcher
{
	virtual const char *name() const override { return "lyricsmania.com"; }
	
protected:
	virtual const char *regex() const override { return "<div class=\"lyrics-body\".*?</strong>(.*?)</div>"; }
};

struct Sing365Fetcher : public GoogleLyricsFetcher
{
	virtual const char *name() const override { return "sing365.com"; }
	
protected:
	virtual const char *regex() const override { return "<!-Lyrics Begin->(.*?)<!-Lyrics End->"; }
};

struct JustSomeLyricsFetcher : public GoogleLyricsFetcher
{
	virtual const char *name() const override { return "justsomelyrics.com"; }
	
protected:
	virtual const char *regex() const override { return "<div class=\"content.*?</div>\\s*</div>(.*?)<div"; }
};

struct AzLyricsFetcher : public GoogleLyricsFetcher
{
	virtual const char *name() const override { return "azlyrics.com"; }
	
protected:
	virtual const char *regex() const override { return "<div class=\"lyricsh\">.*?</h2>.*<div>(.*?)</div>"; }
};

struct GeniusFetcher : public GoogleLyricsFetcher
{
	virtual const char *name() const override { return "genius.com"; }

protected:
	virtual const char *regex() const override { return "<lyrics.*?>(.*?)</lyrics>"; }
};

struct TekstowoFetcher : public GoogleLyricsFetcher
{
	virtual const char *name() const override { return "tekstowo.pl"; }

protected:
	virtual const char *regex() const override { return "<div class=\"song-text\">.*?</h2>(.*?)<a"; }
};

struct InternetLyricsFetcher : public GoogleLyricsFetcher
{
	virtual const char *name() const override { return "the Internet"; }
	virtual Result fetch(const std::string &artist, const std::string &title) override;
	
protected:
	virtual const char *siteKeyword() const override { return "lyrics"; }
	virtual const char *regex() const override { return ""; }
	
	virtual bool isURLOk(const std::string &url) override;
	
private:
	std::string URL;
};

#endif // HAVE_CURL_CURL_H

#endif // NCMPCPP_LYRICS_FETCHER_H
