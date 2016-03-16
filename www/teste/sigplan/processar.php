<?php
####### GNU General Public License #############################################
#                                                                              #
# This file is part of HOA Open Accessibility.                                 #
# Copyright (c) 2007 Ivan ENDERLIN. All rights reserved.                       #
#                                                                              #
# HOA Open Accessibility is free software; you can redistribute it and/or      #
# modify it under the terms of the GNU General Public License as published by  #
# the Free Software Foundation; either version 2 of the License, or            #
# (at your option) any later version.                                          #
#                                                                              #
# HOA Open Accessibility is distributed in the hope that it will be useful,    #
# but WITHOUT ANY WARRANTY; without even the implied warranty of               #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                #
# GNU General Public License for more details.                                 #
#                                                                              #
# You should have received a copy of the GNU General Public License            #
# along with HOA Open Accessibility; if not, write to the Free Software        #
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA   #
#                                                                              #
####### !GNU General Public License ############################################

/**
 * Class Xml.
 *
 * Parse a XML document into a nested array.
 * @author Ivan Enderlin <ivan.enderlin@hoa-project.net>
 * @copyright 2007 Ivan Enderlin.
 * @since PHP4
 * @version 0.4
 * @package Xml
 * @licence GNU GPL
 */

class Xml {

	/**
	 * Xml parser container.
	 *
	 * @var resource parser
	 */
	var $parser;

	/**
	 * Parse result.
	 *
	 * @var array pOut
	 */
	var $pOut = array();

	/**
	 * Contain the overlap tag temporarily .
	 *
	 * @var array track
	 */
	var $track = array();

	/**
	 * Current tag level.
	 *
	 * @var string tmpLevel
	 */
	var $tmpLevel = '';

	/**
	 * Attribut of current tag.
	 *
	 * @var array tmpAttrLevel
	 */
	var $tmpAttrLevel = array();

	/**
	 * Write result.
	 *
	 * @var string wOut
	 */
	var $wOut = '';




	/**
	 * parse
	 * Set the parser Xml and theses options.
	 * Xml file could be a string, a file, or curl.
	 * When the source is loaded, we run the parse.
	 * After, we clean all the memory and variables,
	 * and return the result in an array.
	 *
	 * @access  public
	 * @param   src       string    Source
	 * @param   typeof    string    Source type : NULL, FILE, CURL.
	 * @param   encoding  string    Encoding type.
	 * @return  array
	 */
	function parse ( $src, $typeof = 'FILE', $encoding = 'UTF-8' ) {

		// ini;
		// (re)set array;
		$this->pOut = array();
		$this->parser = xml_parser_create();

		xml_parser_set_option($this->parser, XML_OPTION_CASE_FOLDING, 0);
		xml_parser_set_option($this->parser, XML_OPTION_TARGET_ENCODING, $encoding);

		xml_set_object($this->parser, $this);
		xml_set_element_handler($this->parser, 'startHandler', 'endHandler');
		xml_set_character_data_handler($this->parser, 'contentHandler');

		if(empty($src))
			trigger_error('Source could not be empty.', E_USER_ERROR);     

		// format source;
		if($typeof == NULL)
			$data = $src;
		elseif($typeof == 'FILE') {
			$fop  = fopen($src, 'r');
			$data = null;
			while(!feof($fop))
				$data .= fread($fop, 1024);
			fclose($fop);
		}
		elseif($typeof == 'CURL') {
			$curl = curl_init();
			curl_setopt($curl, CURLOPT_URL, $src);
			curl_setopt($curl, CURLOPT_HEADER, 0);
			$data = curl_exec($curl);
			curl_close($curl);
		}
		else
			return trigger_error('Xml parser need data.', E_USER_ERROR);

		// parse $data;
		$parse = xml_parse($this->parser, $data);
		if(!$parse)
			return trigger_error('XML Error : %s at line %d.', E_USER_ERROR,
				array(xml_error_string(xml_get_error_code($this->parser)),
					xml_get_current_line_number($this->parser)));

		// destroy parser;
		xml_parser_free($this->parser);

		// unset extra vars;
		unset($data,
			  $this->track,
			  $this->tmpLevel);

		// remove global tag and return the result;
		return $this->pOut[0][key($this->pOut[0])];
	}



	/**
	 * startHandler
	 * Manage the open tag, and these attributs by callback.
	 * The purpose is to create a pointer : {{int ptr}}.
	 * If the pointer exists, we have a multi-tag situation.
	 * Tag name  is stocked like : '<tag>'
	 * Attributs is stocked like : '<tag>-ATTR'
	 * Return true but built $this->pOut.
	 *
	 * @access  private
	 * @param   parser  resource    Parser resource.
	 * @param   tag     string      Tag name.
	 * @param   attr    array       Attribut.
	 * @return  bool
	 */
	function startHandler ( $parser, $tag, $attr ) {

		// built $this->track;
		$this->track[] = $tag;
		// place pointer to the end;
		end($this->track);
		// temp level;
		$this->tmpLevel = key($this->track);

		// built attrLevel into $this->tmpAttrLevel
		if(isset($this->tmpAttrLevel[$this->tmpLevel]['attrLevel']))
			$this->tmpAttrLevel[$this->tmpLevel]['attrLevel']++;

		// built $this->pOut;
		if(!isset($this->pOut[key($this->track)][$tag])) {
			$this->pOut[key($this->track)][$tag] = '{{'.key($this->track).'}}';

			if(!isset($this->tmpAttrLevel[$this->tmpLevel]['attrLevel']))
				$this->tmpAttrLevel[$this->tmpLevel]['attrLevel'] = 0;				
		}

		// built attributs;
		if(!empty($attr)) {

			$this->tmpAttrLevel[$this->tmpLevel][] = $this->tmpAttrLevel[$this->tmpLevel]['attrLevel'];
			end($this->tmpAttrLevel[$this->tmpLevel]);

			// it's the first attribut;
			if(!isset($this->pOut[key($this->track)][$tag.'-ATTR']))
					$this->pOut[key($this->track)][$tag.'-ATTR'] = $attr;

			// or it's not the first;
			else {
				// so it's the second;
				if(!prev($this->tmpAttrLevel[$this->tmpLevel])) {
					$this->pOut[key($this->track)][$tag.'-ATTR'] = array(
						current($this->tmpAttrLevel[$this->tmpLevel]) => $this->pOut[key($this->track)][$tag.'-ATTR'],
						next($this->tmpAttrLevel[$this->tmpLevel])    => $attr
					);
				}
				// or one other;
				else
					$this->pOut[key($this->track)][$tag.'-ATTR'][$this->tmpAttrLevel[$this->tmpLevel]['attrLevel']] = $attr;
			}
		}

		return true;
	}



	/**
	 * contentHandler
	 * Detect the pointer, or the multi-tag by callback.
	 * If we have a pointer, the method replaces this pointer by the content.
	 * Else we have a multi-tag, the method add a element to this array.
	 * Return true but built $this->pOut.
	 *
	 * @access  private
	 * @param   parser          resource    Parser resource.
	 * @param   contentHandler  string      Tag content.
	 * @return  bool
	 */
	function contentHandler ( $parser, $contentHandler ) {

		// remove all spaces;
		if(!preg_match('#^\\\\s*$#', $contentHandler)) {

			// $contentHandler is a string;
			if(is_string($this->pOut[key($this->track)][current($this->track)])) {

				// then $contentHandler is a pointer : {{int ptr}}     case 1;
				if(preg_match('#{{([0-9]+)}}#', $this->pOut[key($this->track)][current($this->track)]))
					$this->pOut[key($this->track)][current($this->track)] = $contentHandler;

				// or then $contentHandler is a multi-tag content      case 2;
				else {
					$this->pOut[key($this->track)][current($this->track)] = array(
						0 => $this->pOut[key($this->track)][current($this->track)],
						1 => $contentHandler
					);
				}
			}
			// or $contentHandler is an array;
			else {

				// then $contentHandler is the multi-tag array         case 1;
				if(isset($this->pOut[key($this->track)][current($this->track)][0]))
					$this->pOut[key($this->track)][current($this->track)][] = $contentHandler;

				// or then $contentHandler is a node-tag               case 2;
				else
					$this->pOut[key($this->track)][current($this->track)] = array(
						0 => $this->pOut[key($this->track)][current($this->track)],
						1 => $contentHandler
					);
			}

		}

		return true;
	}



	/**
	 * endHandler
	 * Detect the last pointer by callback.
	 * Move the last tags block up.
	 * And reset some temp variables.
	 * Return true but built $this->pOut.
	 *
	 * @access  private
	 * @param   parser  resource    Parser resource.
	 * @param   tag     string      Tag name.
	 * @return  bool
	 */
	function endHandler ( $parser, $tag ) {

		// if level--;
		if(key($this->track) == $this->tmpLevel-1) {
			// search up tag;
			// use array_keys if an empty tag exists (taking the last tag);

			// if it's a normal framaset;
			$keyBack = array_keys($this->pOut[key($this->track)], '{{'.key($this->track).'}}');
			$count = count($keyBack);

			if($count != 0) {
				$keyBack = $keyBack{$count-1};
				// move this level up;
				$this->pOut[key($this->track)][$keyBack] = $this->pOut[key($this->track)+1];
			}

			// if we have a multi-tag framaset ($count == 0);
			else {
				// if place is set;
				if(isset($this->pOut[key($this->track)][current($this->track)][0])) {

					// if it's a string, we built an array;
					if(is_string($this->pOut[key($this->track)][current($this->track)]))
						$this->pOut[key($this->track)][current($this->track)] = array(
							0 => $this->pOut[key($this->track)][current($this->track)],
							1 => $this->pOut[key($this->track)+1]
						);

					// else add an index into the array;
					else
						$this->pOut[key($this->track)][current($this->track)][] = $this->pOut[key($this->track)+1];
				}
				// else set the place;
				else
					$this->pOut[key($this->track)][current($this->track)] = array(
						0 => $this->pOut[key($this->track)][current($this->track)],
						1 => $this->pOut[key($this->track)+1]
					);
			}

			// kick $this->pOut level out;
			array_pop($this->pOut);
			end($this->pOut);
		}

		// re-temp level;
		$this->tmpLevel = key($this->track);

		while(isset($this->tmpAttrLevel[$this->tmpLevel+1]))
			array_pop($this->tmpAttrLevel);

		// kick $this->track level out;
		array_pop($this->track);
		end($this->track);

		return true;
	}
}

$_REQUEST['baselogin'] = "simec_espelho_producao";

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

restore_error_handler();
restore_exception_handler();
/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "2048M");
set_time_limit( 0 );
/* FIM configurações - Memoria limite de 1024 Mbytes */


$db          = new cls_banco();


$sql = "SELECT prgcod, prgano FROM monitora.programa GROUP BY prgcod, prgano";
$programas = $db->carregar($sql);

header('Content-Type: text/html; charset=utf-8');
$xml = new Xml;

echo "<pre>";

if($programas[0]) {
	foreach($programas as $prg) {
		$source = file_get_contents('http://simec-d/teste/sigplan/ler.php?arquivo=resp_'.$prg['prgano'].'_'.$prg['prgcod'].'.xml');
		$out = $xml->parse($source, NULL);
		$inds = $out['soap:Body']['geracaoPorProgramaResponse']['geracaoPorProgramaResult']['ArrayOfIndicador']['Indicador'];
		if(is_array($inds)) {
			if(!is_array($inds[0])) {
				$x = $inds;
				unset($inds);
				$inds[0] = $x;
			}
			foreach($inds as $i) {
				if($i['INDDsc']) 		  $up[] = "inddsc='".((is_array($i['INDDsc']))?trim(implode("",$i['INDDsc'])):$i['INDDsc'])."'";
				if($i['INDDscFonte'])     $up[] = "inddscfonte='".((is_array($i['INDDscFonte']))?trim(implode("",$i['INDDscFonte'])):$i['INDDscFonte'])."'";
				if($i['INDDscFormula'])   $up[] = "inddscformula='".((is_array($i['INDDscFormula']))?trim(implode("",$i['INDDscFormula'])):$i['INDDscFormula'])."'";
				if($i['INDVlrFinalPrg'])  $up[] = "indvlrfinalprg='".$i['INDVlrFinalPrg']."'";
				if($i['INDVlrApurado'])   $up[] = "indvlrapurado='".$i['INDVlrApurado']."'";
				if($i['INDVlrFinalPPA'])  $up[] = "indvlrfinalppa='".$i['INDVlrFinalPPA']."'";
				if($i['INDVlrPeriodo01']) $up[] = "indvlrperiodo01='".$i['INDVlrPeriodo01']."'";
				if($i['INDVlrPeriodo02']) $up[] = "indvlrperiodo02='".$i['INDVlrPeriodo02']."'";
				if($i['INDVlrPeriodo03']) $up[] = "indvlrperiodo03='".$i['INDVlrPeriodo03']."'";
				if($i['INDVlrPeriodo04']) $up[] = "indvlrperiodo04='".$i['INDVlrPeriodo04']."'";
				if($i['INDVlrPeriodo05']) $up[] = "indvlrperiodo05='".$i['INDVlrPeriodo05']."'";
				if($i['INDVlrPeriodo06']) $up[] = "indvlrperiodo06='".$i['INDVlrPeriodo06']."'";
				if($i['INDVlrPeriodo07']) $up[] = "indvlrperiodo07='".$i['INDVlrPeriodo07']."'";
				if($i['INDVlrPeriodo08']) $up[] = "indvlrperiodo08='".$i['INDVlrPeriodo08']."'";
				if($i['INDVlrPeriodo09']) $up[] = "indvlrperiodo09='".$i['INDVlrPeriodo09']."'";
				if($i['INDVlrPeriodo10']) $up[] = "indvlrperiodo10='".$i['INDVlrPeriodo10']."'";
				if($i['INDVlrPeriodo11']) $up[] = "indvlrperiodo11='".$i['INDVlrPeriodo11']."'";
				if($i['INDVlrPeriodo12']) $up[] = "indvlrperiodo12='".$i['INDVlrPeriodo12']."'";
				if($i['INDDataApuracao']) $up[] = "inddataapuracao='".substr($i['INDDataApuracao'],0,10)."'";
				
				if($up) {
					$sql = "UPDATE monitora.indicador ".(($up)?"SET ".implode(", ",$up):"")." WHERE prgano='".$i['PRGAno']."' AND prgcod='".$i['PRGCod']."' AND indnum='".$i['INDNum']."';";
					echo $sql."<br>";
				}
				unset($up);
			}
		} else {
			echo "-- Sem indicadores (PRGCOD:".$prg['prgcod'].",PRGANO:".$prg['prgano'].")<br>";
		}
	}
}
