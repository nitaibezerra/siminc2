<?php

include_once APPRAIZ . "includes/classes/modelo/questionario/tabelas/Tabela.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/tabelas/Tabelaresposta.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/tabelas/Grupocoluna.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/tabelas/Grupolinha.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/tabelas/Linha.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/tabelas/Coluna.class.inc";

class Montatabela {

	private $htm;

	public function montaNovaTabela( $perid, $qrpid, $percent = 90, $ro = 0 ){

		//global $db;

		//Verifico se existe tabela
		$obTabela 		= new Tabela( );
		$verifica = $obTabela->verificaTabela( $perid );

		$l = 0;

		if( $ro == 1 ){
			$habil = "N";
			$style = "";
		} elseif( $ro == 2 ) {
			$habil = "N";
		} else {
			$habil = "S";
		}

		//Montando a Tabela
		if( $verifica ){

			//DADOS DA TABELA
			$arrDadosTabela = $obTabela->pegaDados( $perid );
			$tabela = $arrDadosTabela['tblid'];
			$percent = $arrDadosTabela['tblpercent'] ? $arrDadosTabela['tblpercent'] : $percent;

			//RESPOSTAS
			$obTabelaResposta = new Tabelaresposta( );
			$arrResp 		  = $obTabelaResposta->recuperarResposta( $tabela, $qrpid );

			//SUBTITULO
			$obGrupoColuna  = new Grupocoluna( );
			$arrGrupoColuna = $obGrupoColuna->pegaDados( $tabela );

			//LINHAS
			$obLinha  		= new Linha( );
			$obGrupolinha  	= new Grupolinha( );
			$arrLinha 		= $obLinha->pegaDados( $tabela );

			//COLUNAS
			$obColuna  = new Coluna( );
			$arrColuna = $obColuna->pegaDados( $tabela );

			$htm .= "<div class=ibox-content >";
			$htm .= "<input type='hidden' name='idTabela' id='idTabela' value='{$tabela}'>";
			$htm .= "<input type='hidden' name='tipoTabela' id='tipoTabela' value='{$arrDadosTabela['tbltipo']}'>";
			$htm .= "<div class=ibox-content style=\"text-align: justify; text-justify: inter-word;\" >{$arrDadosTabela['descricao']}</div>";
			$htm .= "<table class=table>";
			$htm .= "<thead>";
			$htm .= "<tr>";
			$grupo = $obGrupolinha->conta( $tabela );

			$rolspan = $obGrupoColuna->verificaNiveis( $tabela );
			$colspan = $grupo > 1 ? 2 : 1;

			if( $arrDadosTabela['colunas'] && $arrDadosTabela['linhas'] ){
				$htm .= "<td align='center' colspan='$colspan' rowspan='{$rolspan}'><b>{$arrDadosTabela['colunas']} / {$arrDadosTabela['linhas']}</b></td>";
			} else {
				$flag = 2;
			}

			$t = 0;
			$i = 0;
			$grupai = "";
			foreach( $arrGrupoColuna as $subTitulo ){
				if( $subTitulo['grupai'] <> '' ){
					$t = 1;
				}
			}
			foreach( $arrGrupoColuna as $subTitulo ){
				$arrGrcid = $obGrupoColuna->pegaArrUltimoNivel( $subTitulo['codigo'] );

				$col = $obColuna->conta( $arrGrcid );

				if( $arrDadosTabela['colunas'] == '' && $arrDadosTabela['linhas'] == '' && $t == 1 && $i == 0){
					$htm .= "<td align='center'></td>";
				}
				if( $subTitulo['descricao'] <> '&nbsp;' ){
					if( $subTitulo['grupai'] <> '' ){
						// filho
						$htm .= "</tr><tr><td align='center' colspan='{$col}'><b>{$subTitulo['descricao']}</b></td>";
					} else {
						// pai
						$htm .= "<td align='center' colspan='{$col}'><b>{$subTitulo['descricao']}</b></td>";
					}
				}
				$grupai = $subTitulo['grupai'];
				$i++;
			}
			$htm .= "</tr>";
			//Monta as colunas
			$htm .= "<tr>";
			if( $flag == 2 ){
				$htm .= "<td align='center' width='40%'><b>{$arrLinha[0]['grupolinha']}</b></td>";
			}
			foreach( $arrColuna as $coluna ){
				if( $coluna['collegenda'] == '' ){
					$htm .= " <td align='center'><b>{$coluna['descricao']}</b></td>";
				} else {
					$htm .= " <td align='center'><b>{$coluna['descricao']}</b><img title='".$coluna['collegenda']."' src='/imagens/ajuda.gif'></td>";
				}
			}
			$htm .= "</tr>";
			$htm .= "</thead>";
			$htm .= "<tbody>";

			$grp = 0;
			$bgcolor = "$bgcolor2";
			$soma = array();
			foreach( $arrLinha as $linha ){
				$htm .= "<tr>";
				if( $grp <> $linha['grupoid'] ){
					if( $linha['grupolinha'] <> '&nbsp;' && $linha['grupopai'] || $grupo > 1 ){
						$numero = $obLinha->conta( $linha['grupoid'] );
						$htm .= "<td rowspan='{$numero}' width='25%' align='center'><b>{$linha['grupolinha']}</b></td>";
					}
				}
				if( $linha['lindsc'] == 't' ){
					$resposta = isset($arrResp[$linha['codigo']]['']) ? $arrResp[$linha['codigo']][''] : '';
					$htm .= "<td><b>{$linha['descricao']}: ".campo_texto('campo_desc_'.$linha['codigo'],'N',$habil,'',30,160,'','','','','','campo_desc_'.$linha['codigo'], '', $resposta)."</b></td>";
				} else {
					$htm .= "<td><b>{$linha['descricao']}</b></td>";
				}
				$l++;
				$grp = $linha['grupoid'];
				foreach( $arrColuna as $coluna ){
					$c++;
					// O miolo da Tabela
					$resposta = isset($arrResp[$linha['codigo']][$coluna['codigo']]) ? $arrResp[$linha['codigo']][$coluna['codigo']] : '';
					switch ($coluna['coltipo']){
		 			case 'CK':
		 				$habilCK = $habil == "N" ? 'disabled' : '';
		 				$checked = $resposta == 1 ? 'checked="checked"' : '';
						$htm .= "<td align='center'><input type=\"checkbox\" $habilCK name=\"campo_" . $linha['codigo'].'_'.$coluna['codigo'] . "\" value='1' " . $checked . " />";
		 				if( $arrDadosTabela['tbltotalizador'] == 'S' ){
		 					if( $resposta == 1 ){
			 					$soma[$coluna['codigo']][] = 1;
		 					} else {
			 					$soma[$coluna['codigo']][] = 0;
		 					}
		 				}
		 				break;
		 			case 'TX':
		 				$mascara = $coluna['colmascara'] ? $coluna['colmascara'] : '';
		 				if( $arrDadosTabela['tbltotalizador'] == 'S' ){
		 					$soma[$coluna['codigo']][] = $resposta;
		 				}
		 				$max = $coluna['coltamanhomaximo'] ? $coluna['coltamanhomaximo'] : 60;
		 				if( $habil == 'N' ){
		 					$htm .= "<td align='center'>".$resposta."&nbsp;</td>";
		 				} else {
							$htm .= "<td align='center'>".campo_texto('campo_'.$linha['codigo'].'_'.$coluna['codigo'],'N',$habil,'',8,$max,$mascara,'','','','','campo_'.$linha['codigo'].'_'.$coluna['codigo'], '', $resposta)."</td>";
		 				}
		 				break;
		 			case 'RA':
		 				$habilCK = $habil == "N" ? 'disabled': '';
		 				$checked = $resposta == 1 ? 'checked="checked"' : '';
						$htm .= "<td align='center'><input type=\"radio\" $habilCK name=\"campo_" . $linha['codigo'] . "\" value='".$coluna['codigo']."' " . $checked . " />";
		 				break;
		 			}
				}
				$c = 0;
				$htm .= "</tr>";
			}
			// caso tenha que ter totalizador ele gera a última linha
			if( $arrDadosTabela['tbltotalizador'] == 'S' ){
				$htm .= "<tr>";
				$htm .= "<td align='center'><b>TOTAL</b></td>";
				foreach( $soma as $s ){
					foreach( $s as $valores ){
						$sum = $sum + str_replace(',','.',str_replace('.','',$valores));
					}
					$htm .= "<td align='center'><b>".$sum."</b></td>";
					$sum = 0;
				}
				$htm .= "</tr>";
			}
			$htm .= "</tbody>";
			$htm .= "</table>";
			$htm .= "</div>";
			$this->htm = $htm;
		} else {
			$htm .= "<div style='width:100%; height:100%; overflow:auto;'>";
			$htm .= "<table class='tabela' style='width:{$percent}%;' align='center'>";
			$htm .= "<tr>";
			$htm .= "<td bgcolor='$bgcolor1' colspan='50' align='center'><b>AQUI SERÁ MONTADA UMA TABELA. PERID: {$perid}</b></td>";
			$htm .= "</tr>";
			$htm .= "</table>";
			$htm .= "</div>";
			$this->htm = $htm;
		}
	}

	function salvar() {

		$qrpid = $_POST['qrpid'];

		$obTabelaResposta = new Tabelaresposta();
		$obTabelaResposta->excluirPorQuestionarioTabela( $qrpid, $_POST['idTabela'] );
		$obTabelaResposta->commit();

		foreach ($_POST as $chave => $values){
			$arr = array();
			if(strstr($chave,"campo_")){
				if($values != ''){
					$resp = $values;
					$obTabelaResposta = new Tabelaresposta( );
					$arr = explode("_",$chave);
					//if( $_POST['tipoTabela'] == 'RA' ){
					if( !$arr[2] ){ //RADIO
						list( , $linha ) = explode("_",$chave);;
						$coluna = $values;
						$resp = 1;
					} else {
						if( $arr[1] == 'desc' ){ // Campo texto de descrição na LINHA
							list( , $tipo, $linha ) = explode("_",$chave);;
							$coluna = null;
						} else {
							list( , $linha, $coluna ) = explode("_",$chave);;
						}
					}

					$arrCampos = array('tblid', 'linid', 'colid', 'qrpid', 'tbrdsc');
					$arrDados  = array("tblid" => $_POST['idTabela'],
										  "linid" => $linha,
										  "colid" => $coluna,
										  "qrpid" => $qrpid,
										  "tbrdsc" => $resp);
					$obTabelaResposta->popularObjeto( $arrCampos, $arrDados );
					$obTabelaResposta->salvar();
					$obTabelaResposta->commit();
				}
			}
		}
	}

	function show(){
		echo $this->htm;
	}
}

?>