<?php

/**
 * Função que gera excel a partir de uma consulta sql ou array de dados
 * 
 * @author Wescley Lima <wescley.lima@mec.gov.br>
 * 
 * @param string|array $sql Pode ser uma consulta sql ou um array de dados
 * @param array	$arCabecalho Serve para persdonalizar o cabeçalho, case não seja informado irá pegar o nome das colunas ou indice do array
 * @param string $nome_arq Nome do arquivo a ser gerado sem extensão
 * @param array $arFormatoColuna Tipos das colunas do excel, colocar no array na mesma posição da coluna, array('date', 'text', 'percent', 'int', 'account', 'money')
 * @param boolean $debug Exibe o código html gerado na função
 * @return mixed
 * 
 * @version 0.1
 * @global object $db
 * @example maismedicos/modulos/relatorio/listaNominalMedicosSemVisista_result.inc
 * 
 */

function gerar_excel($sql, $arCabecalho = array(), $nome_arq = '', $arFormatoColuna = array(), $csv = false, $debug = false)
{
	global $db;
	
	// Verifica fonte dos dados, se eh um array ou sql
	if(is_array($sql)){
		$rs = $sql;
	}else{
		$rs = $db->carregar($sql);
	}
	
	// Se nao retornar nada para por aqui
	if(empty($rs) && empty($rs[0])) return false;
	
// 	$cssTabela = "font-family: calibri, san-serif; font-size:9pt;padding:0;margin:0;";
	
	$table  = '<table style="'.$cssTabela.'" width="100%" cellpadding="0" cellspacing="0" border="1">';
	$table .= '<tr>';
	
	// Monta cabecalho conforme array ou conforme colunas da consulta
	if(!empty($arCabecalho)){		
		foreach($arCabecalho as $cabecalho){
			$table .= '<td>'.$cabecalho.'</td>';
		}		
	}else{		
		foreach($rs[0] as $k => $v){
			$table .= '<td>'.$k.'</td>';
		}		
	}
	$table .= '</tr>';
	
	// Relacao dos tipos para as colunas do excel
// 	$cssTpComun 	= 'white-space: nowrap;';
// 	$cssTpDate 		= 'mso-number-format: "Short Date";';
// 	$cssTpText 		= 'mso-number-format: "\@";';
// 	$cssTpPercent 	= 'mso-number-format: "Percent"; text-align:right;';
// 	$cssTpInt 		= 'mso-number-format: "\#\,\#\#0"; text-align:right;';
// 	$cssTpAccount 	= 'mso-number-format: "\#\,\#\#0;[Red\]\(\#\,\#\#0\);\-"; text-align:right;';
// 	$cssTpMoney 	= 'mso-number-format: "Currency"; text-align:right;';
	
	// Monta linhas da tabela
	if($rs){
		foreach($rs as $linha){
			$table .= '<tr>'; $nucoluna=0;
			foreach($linha as $coluna){
				
// 				$tp = $arFormatoColuna[$nucoluna];
// 				$varTp = 'cssTp'.ucwords(strtolower($tp));
				
// 				$table .= '<td style=\''.(isset(${$varTp}) ? ${$varTp} : '').$cssTpComun.'\'>'.$coluna.'</td>';
				$table .= '<td>'.$coluna.'</td>';
				$nucoluna++;
			}
			$table .= '</tr>';
		}
	}
	$table .= '</table>';
	
	// Mostra codigo gerado para debugar
// 	if($debug){ echo simec_htmlentities($table);die; }
	
	$nome_arq = $nome_arq ? $nome_arq : 'Planilha_Simec_'.date('YmdHis');
	$extensao = $csv ? 'csv' : 'xls';
	
	ob_clean();
	
	// Cabecalho para gerar excel
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/{$extensao};");
// 	header ("Content-type: application/x-msexcel");	
	header ( "Content-Disposition: attachment; filename={$nome_arq}.{$extensao}");
	header ( "Content-Description: MID Gera {$extensao}" );
	
	// Imprime tabela
	echo $table;	
	
} 

function monta_lista_tabulado_mm($sql,$cabecalho="",$perpage,$pages,$soma='N',$largura='95%', $valormonetario='S', $totalregistro=false , $arrHeighTds = false , $heightTBody = false, $boImprimiTotal = false ) {
	if(!(bool)$largura) $largura = '95%';
	// este método monta uma listagem na tela baseado na sql passada
	//Registro Atual (instanciado na chamada)
	if ($_REQUEST['numero']=='') $numero = 1; else $numero = intval($_REQUEST['numero']);

	if (is_array($sql))
		$RS = $sql;
	else
		$RS = $this->carregar($sql);

	$nlinhas = $RS ? count($RS) : 0;
	$totalRegistro = $nlinhas;
	if (! $RS) $nl = 0; else $nl=$nlinhas;
	if (($numero+$perpage)>$nlinhas) $reg_fim = $nlinhas; else $reg_fim = $numero+$perpage-1;

	//			$header .= '<table>';
		
	//Monta Cabeçalho
	if(is_array($cabecalho))
	{
		//				$header.= '<tr>';
		//				$header.= chr(13).chr(10);
		for ($i=0;$i<count($cabecalho);$i++)
		{
		$header.= $cabecalho[$i].(($i===$totalregistro)?' (Total:'.count($RS).')':''). chr(9);
		}
		}
			
		//			$header.= "</table>";
			
		if ($nlinhas>0)
		{

		//				$body.= '<table>';

			//Monta Listagem
			$totais = array();
			$tipovl = array();
			for ($i=($numero-1);$i<$reg_fim;$i++)
			{
			$c = 0;
			$body .= chr(13);
			foreach($RS[$i] as $k=>$v) {
			if(is_numeric($v)){
				$v = str_replace(array(",","."),array("",","),$v);
				}
				$body.= str_replace(array(chr(10),chr(13),chr(11),chr(12),chr(9)), ' ',$v);
				$body .= chr(9);
				$c = $c + 1;
			}
			//					$body .= chr(13);
			}

			//		        $body.= '</table>';

		}
			
		echo $header;
		echo $body;
			
	}


	function arrayPerfil(){
		
		global $db;
	
		$sql = "select pu.pflcod from
					seguranca.perfilusuario pu
				inner join
					seguranca.perfil p on p.pflcod = pu.pflcod
				and
					pu.usucpf = '{$_SESSION['usucpf']}'
				and
					p.sisid = {$_SESSION['sisid']}
				and
					pflstatus = 'A'";
	
		$arrPflcod = $db->carregar($sql);
	
		!$arrPflcod? $arrPflcod = array() : $arrPflcod = $arrPflcod;
	
		foreach($arrPflcod as $pflcod){
			$arrPerfil[] = $pflcod['pflcod'];
		}
	
		return $arrPerfil ? $arrPerfil : false;
	}