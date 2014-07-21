//   mlwavedata.cpp
//
//   A Container Class for Audio Meta Data.
//
//   (C) Copyright 2002-2006 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mlwavedata.cpp,v 1.3 2012/06/26 21:30:02 cvs Exp $
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Library General Public License 
//   version 2 as published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//

#include "mlwavedata.h"


MLWaveData::MLWaveData()
{
  clear();
}


bool MLWaveData::metadataFound() const
{
  return data_metadata_found;
}


void MLWaveData::setMetadataFound(bool state)
{
  data_metadata_found=state;
}


QString MLWaveData::title() const
{
  return data_title;
}


void MLWaveData::setTitle(const QString &str)
{
  data_title=str;
}


QString MLWaveData::artist() const
{
  return data_artist;
}


void MLWaveData::setArtist(const QString &str)
{
  data_artist=str;
}


QString MLWaveData::album() const
{
  return data_album;
}


void MLWaveData::setAlbum(const QString &str)
{
  data_album=str;
}


QString MLWaveData::label() const
{
  return data_label;
}


void MLWaveData::setLabel(const QString &str)
{
  data_label=str;
}


int MLWaveData::releaseYear() const
{
  return data_release_year;
}


void MLWaveData::setReleaseYear(int year)
{
  data_release_year=year;
}


QString MLWaveData::client() const
{
  return data_client;
}


void MLWaveData::setClient(const QString &str)
{
  data_client=str;
}


QString MLWaveData::agency() const
{
  return data_agency;
}


void MLWaveData::setAgency(const QString &str)
{
  data_agency=str;
}


QString MLWaveData::composer() const
{
  return data_composer;
}


void MLWaveData::setComposer(const QString &str)
{
  data_composer=str;
}


QString MLWaveData::publisher() const
{
  return data_publisher;
}


void MLWaveData::setPublisher(const QString &str)
{
  data_publisher=str;
}


QString MLWaveData::licensingOrganization() const
{
  return data_licensing_organization;
}


void MLWaveData::setLicensingOrganization(const QString &str)
{
  data_licensing_organization=str;
}


QString MLWaveData::copyrightNotice() const
{
  return data_copyright_notice;
}


void MLWaveData::setCopyrightNotice(const QString &str)
{
  data_copyright_notice=str;
}


QString MLWaveData::cutId() const
{
  return data_cut_id;
}


void MLWaveData::setCutId(const QString &str)
{
  data_cut_id=str;
}


QString MLWaveData::classification() const
{
  return data_classification;
}


void MLWaveData::setClassification(const QString &str)
{
  data_classification=str;
}


QString MLWaveData::category() const
{
  return data_category;
}


void MLWaveData::setCategory(const QString &str)
{
  data_category=str;
}


QString MLWaveData::url() const
{
  return data_url;
}


void MLWaveData::setUrl(const QString &str)
{
  data_url=str;
}


QString MLWaveData::tagText() const
{
  return data_tag_text;
}


void MLWaveData::setTagText(const QString &str)
{
  data_tag_text=str;
}


QString MLWaveData::description() const
{
  return data_description;
}


void MLWaveData::setDescription(const QString &str)
{
  data_description=str;
}


QString MLWaveData::originator() const
{
  return data_originator;
}


void MLWaveData::setOriginator(const QString &str)
{
  data_originator=str;
}


QString MLWaveData::originatorReference() const
{
  return data_originator_reference;
}


void MLWaveData::setOriginatorReference(const QString &str)
{
  data_originator_reference=str;
}


QString MLWaveData::codingHistory() const
{
  return data_coding_history;
}


void MLWaveData::setCodingHistory(const QString &str)
{
  data_coding_history=str;
}


QString MLWaveData::userDefined() const
{
  return data_user_defined;
}


void MLWaveData::setUserDefined(const QString &str)
{
  data_user_defined=str;
}


QString MLWaveData::isrc() const
{
  return data_isrc;
}


void MLWaveData::setIsrc(const QString &str)
{
  data_isrc=str;
}


QString MLWaveData::mcn() const
{
  return data_mcn;
}


void MLWaveData::setMcn(const QString &str)
{
  data_mcn=str;
}


QString MLWaveData::outCue() const
{
  return data_out_cue;
}


void MLWaveData::setOutCue(const QString &str)
{
  data_out_cue=str;
}


MLWaveData::EndType MLWaveData::endType() const
{
  return data_end_type;
}


void MLWaveData::setEndType(MLWaveData::EndType type)
{
  data_end_type=type;
}


int MLWaveData::introStartPos() const
{
  return data_intro_start_pos;
}


void MLWaveData::setIntroStartPos(int msec)
{
  data_intro_start_pos=msec;
}


int MLWaveData::introEndPos() const
{
  return data_intro_end_pos;
}


void MLWaveData::setIntroEndPos(int msec)
{
  data_intro_end_pos=msec;
}


int MLWaveData::segueStartPos() const
{
  return data_segue_start_pos;
}


void MLWaveData::setSegueStartPos(int msec)
{
  data_segue_start_pos=msec;
}


int MLWaveData::segueEndPos() const
{
  return data_segue_end_pos;
}


void MLWaveData::setSegueEndPos(int msec)
{
  data_segue_end_pos=msec;
}


int MLWaveData::startPos() const
{
  return data_start_pos;
}


void MLWaveData::setStartPos(int msec)
{
  data_start_pos=msec;
}


int MLWaveData::endPos() const
{
  return data_end_pos;
}


void MLWaveData::setEndPos(int msec)
{
  data_end_pos=msec;
}


int MLWaveData::beatsPerMinute() const
{
  return data_beats_per_minute;
}


void MLWaveData::setBeatsPerMinute(int bpm)
{
  data_beats_per_minute=bpm;
}


QString MLWaveData::tmciSongId() const
{
  return data_tmci_song_id;
}


void MLWaveData::setTmciSongId(const QString &str)
{
  data_tmci_song_id=str;
}


QDate MLWaveData::startDate() const
{
  return data_start_date;
}


void MLWaveData::setStartDate(const QDate &date)
{
  data_start_date=date;
}


QTime MLWaveData::startTime() const
{
  return data_start_time;
}


void MLWaveData::setStartTime(const QTime &time)
{
  data_start_time=time;
}


QDate MLWaveData::originationDate() const
{
  return data_origination_date;
}


void MLWaveData::setOriginationDate(const QDate &date)
{
  data_origination_date=date;
}


QTime MLWaveData::originationTime() const
{
  return data_origination_time;
}


void MLWaveData::setOriginationTime(const QTime &time)
{
  data_origination_time=time;
}


QDate MLWaveData::endDate() const
{
  return data_end_date;
}


void MLWaveData::setEndDate(const QDate &date)
{
  data_end_date=date;
}


QTime MLWaveData::endTime() const
{
  return data_end_time;
}


void MLWaveData::setEndTime(const QTime &time)
{
  data_end_time=time;
}


void MLWaveData::clear()
{
  data_metadata_found=false;
  data_title="";
  data_artist="";
  data_album="";
  data_label="";
  data_release_year=0;
  data_client="";
  data_agency="";
  data_composer="";
  data_publisher="";
  data_licensing_organization="";
  data_copyright_notice="";
  data_user_defined="";
  data_cut_id="";
  data_classification="";
  data_category="";
  data_url="";
  data_tag_text="";
  data_description="";
  data_originator="";
  data_originator_reference="";
  data_isrc="";
  data_mcn="";
  data_out_cue="";
  data_end_type=MLWaveData::UnknownEnd;
  data_intro_start_pos=-1;
  data_intro_end_pos=-1;
  data_segue_start_pos=-1;
  data_segue_end_pos=-1;
  data_start_pos=-1;
  data_end_pos=-1;
  data_beats_per_minute=0;
  data_tmci_song_id="";
  data_origination_date=QDate();
  data_origination_time=QTime();
  data_start_date=QDate();
  data_start_time=QTime();
  data_end_date=QDate();
  data_end_time=QTime();
}
