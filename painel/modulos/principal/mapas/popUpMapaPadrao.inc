<?php
$_POST['excel'] = !$_POST['excel'] ? "" : $_POST['excel'];
$camada_filtros_avancados = str_replace("tipmunici_","",$_POST['camada_filtros_avancados']);
$camada_filtros_avancados = $camada_filtros_avancados ? $camada_filtros_avancados : "municipio";
$_POST['agrupador'] = !$_POST['agrupador'] ? $camada_filtros_avancados : $_POST['agrupador'];
$camada = "municipio";

if($_POST['excel'] != ""){
	$mapid = $_GET['mapid'];
	extract($_POST);
	if(is_numeric($estuf)){
		$estuf = ("select estuf from territorios.estado where regcod = '".str_replace("estuf","",$estuf)."'");
		$arrWhereUF = "and mun.estuf in ($estuf)";
	}else{
		if(strstr($estuf,"tipo")){
			$arrWhereUF = "and mun.muncod in (select distinct mtm2.muncod from territoriosgeo.muntipomunicipio mtm2 where mtm2.tpmid = ".str_replace("tipo","",$estuf).")";
		}else{
			$estuf = str_replace("estuf","",$estuf);
			if($estuf){
				$arrUF = explode(",",$estuf);
				$arrWhereUF = "and mun.estuf in ('".implode("','",$arrUF)."')";
			}	
		}
	}
	
	if($_POST['agrupador'] == "estado"){
		$arrCabecalho[] = "Estado";
		$arrCabecalho[] = "Qtde Município(s)";
	}elseif($_POST['agrupador'] == "regiao"){
		$arrCabecalho[] = "Região";
		$arrCabecalho[] = "Qtde Município(s)";
	}elseif($_POST['agrupador'] == "municipio"){
		$arrCabecalho[] = "IBGE";
		$arrCabecalho[] = "Município";
		$arrCabecalho[] = " UF";	
	}else{
		$sql_gt = "select gtmdsc from territoriosgeo.grupotipomunicipio where gtmid = {$_POST['agrupador']}";
		$descricao_gtm = $db->pegaUm($sql_gt);
		$arrCabecalho[] = "$descricao_gtm";
		$arrCabecalho[] = "Qtde Município(s)";
	}
		
	$n=1;
	foreach($cmb_tema as $grupo => $arrGrupo){
		foreach($arrGrupo as $tipoFiltro => $tmaid){
			$condicaoDentroGrupo = $cmb_grupo[$grupo][$tipoFiltro];
			$condicao = $cmb_filtro[$grupo][$tipoFiltro];
			$valorCondicaoInicio = str_replace(array(".",",","%"),array("",".",""),$cmb_filtro_inicio[$grupo][$tipoFiltro]);
			$valorCondicaoFim = str_replace(array(".",",","%"),array("",".",""),$cmb_filtro_fim[$grupo][$tipoFiltro]);
			
			$sql1 = "select tmaformula,tpdid from mapa.tema where tmaid = ".$cmb_tema[$grupo][$tipoFiltro];
			$arrTma = $db->pegaLinha($sql1);
			$tpdid = $arrTma['tpdid'];
			$tmaformula = $arrTma['tmaformula'];
			if($tpdid == 6){
				include_once APPRAIZ . 'www/painel/_funcoes_formula_tema.php';
				if($_POST['agrupador'] != "municipio"){
					if($_POST['agrupador'] == "regiao"){
						$formula_coluna = retornaFormulaPopUp($tmaformula,"regiao");
					}elseif($_POST['agrupador'] == "estado"){
						$formula_coluna = retornaFormulaPopUp($tmaformula,"estado");
					}else{
						$formula_coluna = retornaFormulaPopUp($tmaformula,"tpm");
					}
					$arrHaving[] = $formula_coluna['coluna'];
					$arrCampos[] = " round(".$formula_coluna['coluna'].",2)  as coluna{$n} ";
					if($formula_coluna['leftjoin']){
						foreach($formula_coluna['leftjoin'] as $left){
							$arrInnerTema[] = $left;
						}
					}
					$arrWhereFormula[$n] = true;
				}
				if($_POST['agrupador'] == "municipio"){
					$tpdid = atualizaDadosTemaFormula($cmb_tema[$grupo][$tipoFiltro],$_POST['agrupador']);
				}
			}
			
			$arrCondicaoDentroGrupoHaving[] = $condicaoDentroGrupo;
			$arrCondicaoHaving[] = $condicao;
			$arrValorInicialHaving[] = $valorCondicaoInicio;
			$arrValorFinalHaving[] = $valorCondicaoFim;
						
			if($tpdid == 3){
				$arrInnerTema[] = "mapa.temadado  tem_{$n} ON tem_{$n}.muncod = mun.muncod ".($tmaid == "1" ? "" : "and tem_{$n}.tmaid = $tmaid")." $arrWhereUF";
				
				if($_POST['agrupador'] != "municipio"){
					$arrCampos[] = "'-'";
				}else{
					$arrCampos[] = "(CASE WHEN tem_{$n}.tmdboleano is true THEN 'Sim' ELSE 'Não' END) as coluna{$n}";
				}
				
				$arrWhere[$grupo][] = "$condicaoDentroGrupo COALESCE(tem_{$n}.tmdboleano,false) is $condicao";
				$tmdsc = $db->pegaUm("select tmadsc from mapa.tema where tmaid = $tmaid");
				if($condicao == "true"){
					switch($condicaoDentroGrupo){
						case "and":
						$condicao_extenso = "e";
						break;
						case "or":
						$condicao_extenso = "ou";
						break;
						default:
						$condicao_extenso = "";
						break;
					}
					$arrFormula[$grupo][] = "<b>$condicao_extenso</b> $tmdsc ";
					$arrCabecalho[] = $tmdsc;
				}else{
					switch($condicaoDentroGrupo){
						case "and":
						$condicao_extenso = "e";
						break;
						case "or":
						$condicao_extenso = "ou";
						break;
						default:
						$condicao_extenso = "";
						break;
					}
					$arrFormula[$grupo][] = "<b>$condicao_extenso</b> Não $tmdsc ";
					$arrCabecalho[] = $tmdsc;	
				}
				if($tipoFiltro == 1){
					$condicaoGrupo = str_replace(array("and","or"),array("e ","ou "),$cmb_entregrupo[$grupo]);
					$arrCabecalho[] = $tmdsc;
				}
			}else{
				if($tmaid != "1"){
					$arrInnerTema[] = "mapa.temadado  tem_{$n} ON tem_{$n}.muncod = mun.muncod ".($tmaid == "1" ? "" : "and tem_{$n}.tmaid = $tmaid")." $arrWhereUF";
					if($_POST['agrupador'] != "municipio"){
						if($tpdid == 2 || $tpdid == 1){
							$arrCampos[] = "sum(COALESCE(tem_{$n}.tmdvalor,0))::integer as coluna{$n}";
							$arrHaving[] = "sum(COALESCE(tem_{$n}.tmdvalor,0))::integer";
						}else{
							if(!$formula_coluna){
								$arrCampos[] = "'<center>-</center>' as coluna{$n}";
							}else{
								$formula_coluna = false;
							}
						}
					}else{
						if($tpdid == 4){
							$arrCampos[] = "COALESCE(tem_{$n}.tmdvalor,0) as coluna{$n}";
						}else{
							$arrCampos[] = "COALESCE(tem_{$n}.tmdvalor,0)::integer as coluna{$n}";
						}
						
					}
					if($condicao == "entre"){
						if( !$_POST['agrupador'] || ($_POST['agrupador'] != "municipio" && ($tpdid == 2 || $tpdid == 1)) ){
							$arrWhereExterno[$grupo][] = "$condicaoDentroGrupo coluna{$n} between $valorCondicaoInicio and $valorCondicaoFim";
						}
						$arrWhere[$grupo][] = "$condicaoDentroGrupo COALESCE(tem_{$n}.tmdvalor,0) between $valorCondicaoInicio and $valorCondicaoFim";
					}else{
						if( !$_POST['agrupador'] || ($_POST['agrupador'] != "municipio" && ($tpdid == 2 || $tpdid == 1)) ){
							$arrWhereExterno[$grupo][] = "$condicaoDentroGrupo coluna{$n} $condicao $valorCondicaoInicio";
						}
						$arrWhere[$grupo][] = "$condicaoDentroGrupo COALESCE(tem_{$n}.tmdvalor,0) $condicao $valorCondicaoInicio";
					}
				}else{
					$arrCampos[] = "mun.munpopulacao as coluna{$n}";
					if($condicao == "entre"){
						$arrWhere[$grupo][] = "$condicaoDentroGrupo mun.munpopulacao between $valorCondicaoInicio and $valorCondicaoFim";
					}else{
						$arrWhere[$grupo][] = "$condicaoDentroGrupo mun.munpopulacao $condicao $valorCondicaoInicio";
					}
				}	
				
				$tmdsc = $db->pegaUm("select tmadsc from mapa.tema where tmaid = $tmaid");
				
				if($condicao == "entre"){
					switch($condicaoDentroGrupo){
						case "and":
						$condicao_extenso = "e";
						break;
						case "or":
						$condicao_extenso = "ou";
						break;
						default:
						$condicao_extenso = "";
						break;
					}
					$arrFormula[$grupo][] = "<b>$condicao_extenso</b> $tmdsc entre $valorCondicaoInicio e $valorCondicaoFim";
				}else{
					switch($condicaoDentroGrupo){
						case "and":
						$condicao_extenso = "e";
						break;
						case "or":
						$condicao_extenso = "ou";
						break;
						default:
						$condicao_extenso = "";
						break;
					}
					$arrFormula[$grupo][] = "<b>$condicao_extenso</b> $tmdsc $condicao $valorCondicaoInicio";
				}
	
				$arrCabecalho[] = $tmdsc;
			}
			$n++;
		}
	}
	
	$arrWhere = !$arrWhere ? array(): $arrWhere; 
	foreach($arrWhere as $grupo => $arrGrupo){
		if($cmb_entregrupo[$grupo]){
			if($arrWhereExterno[$grupo]){
				$sqlExterno.= " ) ".$cmb_entregrupo[$grupo]." (";	
			}else{
				$sql.= " ) ".$cmb_entregrupo[$grupo]." (";	
			}
			switch($cmb_entregrupo[$grupo]){
				case "and":
				$condicao_extenso = "e";
				break;
				case "or":
				$condicao_extenso = "ou";
				break;
			}
			$formula.= " ) <br /> <b>".$condicao_extenso."</b> <br /> <b>Grupo $grupo:</b> (";
		}
		if($arrWhereExterno[$grupo]){
			foreach($arrWhereExterno[$grupo] as $whereExterno){
				$sqlExterno.= " ".$whereExterno." ";
			}	
		}else{
			foreach($arrWhere[$grupo] as $where){
				$sql.= " ".$where." ";
			}	
		}
	}
	$sql = "( $sql )";
	$sql = str_replace(array("and (  )","(  )","( )"),array("","(1=1)","(1=1)"),$sql);
	$sqlExterno = "( $sqlExterno )";
	$sqlExterno = str_replace(array("(  )","( )"),array("(1=1)","(1=1)"),$sqlExterno);
	$sql = str_replace("select u.usuchaveativacao from seguranca.usuario u where u.usucpf = '{$_SESSION['usucpf']}'","",$sql);
	
	//Agrupadores
	switch($_POST['agrupador']){
		
		case "estado":
			if($camada != "10"){
				if($arrCampos){
					$n=1;
					if($arrWhere[1]){
						foreach($arrWhere[1] as $where){
							if(!$arrWhereFormula[$n]){
								$where_interno_agrupador .= " and ".$arrWhere[1][$n-1];
								$where_interno_agrupador = str_replace( array("sum(COALESCE(" , "))"), array(" "," ") , $where_interno_agrupador);
							}else{
								$arrWhereExternoFormula[] = str_replace("tem_$n.tmdvalor","coluna$n",$arrWhere[1][$n-1]);
							}
							$n++;
						}
					}
				}
				$sqlNovo = "select * from ( select distinct
					est.estdescricao as estado,
					count( distinct mun.muncod) || ' município(s)' as qtde_mun
					".($arrCampos ? ",".implode(",",$arrCampos)."" : "")."
				from 
					territoriosgeo.municipio mun
				inner join
					territoriosgeo.estado est ON est.estuf = mun.estuf
				".($arrInnerTema ? " left join ".implode(" left join ",$arrInnerTema) : "")."
				where ".($arrWhereUF ? " 1=1 ".$arrWhereUF." " : "")."
				";
								
				$sqlNovo = str_replace( array( "mun.munpopulacao","COALESCE(" , ",0)" ) , array( "sum(mun.munpopulacao)","sum(COALESCE(" , ",0))" ) , $sqlNovo);
				
				$sqlNovo.= "$where_interno_agrupador";
				
				$sqlNovo .= " group by est.estdescricao,est.estuf";
				
				if(!strstr(strtoupper($sqlNovo),"ORDER BY")){
					//$sqlNovo .= " order by estado";
				}
				$sqlNovo .= ") as foo where 1=1 ".($arrWhereExternoFormula ? " and ".implode(" ",$arrWhereExternoFormula) : "")." ";
			}else{
				$sqlNovo = "select * from ( select distinct
					est.estdescricao as estado,
					count(mun.muncod) || ' município(s)' as qtde_mun
					".($arrCampos ? ",".implode(",",$arrCampos)."" : "")."
				from 
					territoriosgeo.municipio mun
				inner join
					territoriosgeo.estado est ON est.estuf = mun.estuf
				".($arrInnerTema ? " left join ".implode(" left join ",$arrInnerTema) : "")."
				where
				";
				
				$sqlNovo = str_replace( array( "mun.munpopulacao","COALESCE(" , ",0)" ) , array( "sum(mun.munpopulacao)","sum(COALESCE(" , ",0))" ) , $sqlNovo);
				
				$sqlNovo .= $sql . " group by est.estdescricao,est.estuf order by est.estdescricao";
				$sqlNovo .= ") as foo where $sqlExterno";
			}
			break;
		case "regiao":
			if($camada != "8"){
				if($arrCampos){
					$n=1;
					if($arrWhere[1]){
						foreach($arrWhere[1] as $where){
							if(!$arrWhereFormula[$n]){
								$where_interno_agrupador .= " and ".$arrWhere[1][$n-1];
								$where_interno_agrupador = str_replace( array("sum(COALESCE(" , "))"), array(" "," ") , $where_interno_agrupador);
							}else{
								$arrWhereExternoFormula[] = str_replace("tem_$n.tmdvalor","coluna$n",$arrWhere[1][$n-1]);
							}
							$n++;
						}
					}
				}
				
				$sqlNovo = "select * from ( select distinct
					reg.regdescricao as regiao,
					count(distinct mun.muncod) || ' município(s)' as qtde_mun
					".($arrCampos ? ",".implode(",",$arrCampos)."" : "")."
				from 
					territoriosgeo.municipio mun
				inner join
					territoriosgeo.estado est ON est.estuf = mun.estuf
				inner join
					territorios.regiao reg ON reg.regcod = est.regcod
				".($arrInnerTema ? " left join ".implode(" left join ",$arrInnerTema) : "")."
				where ".($arrWhereUF ? " 1=1 ".$arrWhereUF." " : "")."
				";
				
				$sqlNovo = str_replace( array( "mun.munpopulacao","COALESCE(" , ",0)" ) , array( "sum(mun.munpopulacao)","sum(COALESCE(" , ",0))" ) , $sqlNovo);
				$sqlNovo .="$where_interno_agrupador";
				$sqlNovo .= " group by reg.regdescricao,reg.regcod";
				
				if(!strstr(strtoupper($sqlNovo),"ORDER BY")){
					//$sqlNovo .= " order by regiao";
				}
				$sqlNovo .= ") as foo where 1=1 ".($arrWhereExternoFormula ? " and ".implode(" ",$arrWhereExternoFormula) : "")." ";
			}else{
				$sqlNovo = "select * from ( select distinct
					reg.regdescricao as regiao,
					count(mun.muncod) || ' município(s)' as qtde_mun
					".($arrCampos ? ",".implode(",",$arrCampos)."" : "")."
				from 
					territoriosgeo.municipio mun
				inner join
					territoriosgeo.estado est ON est.estuf = mun.estuf
				inner join
					territorios.regiao reg ON reg.regcod = est.regcod
				".($arrInnerTema ? " left join ".implode(" left join ",$arrInnerTema) : "")."
				where
				";
				
				$sqlNovo = str_replace( array( "mun.munpopulacao","COALESCE(" , ",0)" ) , array( "sum(mun.munpopulacao)","sum(COALESCE(" , ",0))" ) , $sqlNovo);
				
				$sqlNovo .= $sql . " group by reg.regdescricao,reg.regcod order by reg.regdescricao";
				$sqlNovo .= ") as foo where $sqlExterno";
			}
			break;
		case "municipio":
			$sqlNovo = "select * from ( select distinct
				mun.muncod,
				mun.mundescricao,
				mun.estuf
				".($arrCampos ? ",".implode(",",$arrCampos) : "")."
			from 
				territoriosgeo.municipio mun
			".($arrInnerTema ? " left join ".implode(" left join ",$arrInnerTema) : "")."
			where 1=1 ".($arrWhereUF ? $arrWhereUF :"") ." and  
			$sql";
			
			if(!strstr(strtoupper($sqlNovo),"ORDER BY")){
				//$sqlNovo .= " order by mundesc";
			}
			$sqlNovo .= ") as foo where $sqlExterno";
		break;
		default:
			
			$camada_filtros_avancados = str_replace("tipmunici_","",$camada_filtros_avancados);
			if($camada_filtros_avancados != "municipio" && $camada_filtros_avancados != ""){
				if($arrCampos){
					$n=1;
					if($arrWhere){
						foreach($arrWhere as $grupo => $arrW){
							foreach($arrW as $where){
								if(!strstr($where,"tmdboleano")){
									if($arrCondicaoHaving[$n-1] == "entre"){
										$arrHavingFinal[] = $arrCondicaoDentroGrupoHaving[$n-1]." ".$arrHaving[$n-1]." between ".$arrValorInicialHaving[$n-1]." and ".$arrValorFinalHaving[$n-1];
									}else{
										$arrHavingFinal[] = $arrCondicaoDentroGrupoHaving[$n-1]." ".$arrHaving[$n-1]." ".$arrCondicaoHaving[$n-1]." ".$arrValorInicialHaving[$n-1];
									}
								}
								$n++;
							}
							if($arrHavingFinal){
								$having_final.= " (".implode(" ",$arrHavingFinal).") ".$cmb_entregrupo[$grupo+1];
							}
							unset($arrHavingFinal);
						}
					}
				}
				
				$arrInnerTema = array_unique($arrInnerTema);
				$sqlNovo = "
							select 
								tpm.tpmdsc as tipo,
								count(mun.muncod) || ' município(s)' as qtde_mun
								".($arrCampos ? ",".implode(",",$arrCampos)."" : "")."
							from 
							territoriosgeo.municipio mun
							inner join
							territoriosgeo.muntipomunicipio mtm ON mtm.muncod = mun.muncod
							inner join
							territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid 
							".($arrInnerTema ? " left join ".implode(" left join ",$arrInnerTema) : "")."
							where
							tpm.gtmid = {$_POST['agrupador']} and tpm.tpmstatus = 'A' ".($arrWhereUF ? $arrWhereUF : "")."
							group by tpm.tpmdsc,tpm.tpmid
							".($having_final ? "having ".$having_final."" : "")."";
			}else{
				if($arrCampos){
					$n=1;
					if($arrWhere){
						foreach($arrWhere as $grupo => $arrW){
							foreach($arrW as $where){
								if(!strstr($where,"tmdboleano")&& !strstr($where,"munpopulacao")){
									if($arrCondicaoHaving[$n-1] == "entre"){
										$arrHavingFinal[] = $arrCondicaoDentroGrupoHaving[$n-1]." coalesce(tem_".($n).".tmdvalor,0) between ".$arrValorInicialHaving[$n-1]." and ".$arrValorFinalHaving[$n-1];
									}else{
										$arrHavingFinal[] = $arrCondicaoDentroGrupoHaving[$n-1]." coalesce(tem_".($n).".tmdvalor,0) ".$arrCondicaoHaving[$n-1]." ".$arrValorInicialHaving[$n-1];
									}
								}
								if(strstr($where,"munpopulacao")){
									if($arrCondicaoHaving[$n-1] == "entre"){
										$arrHavingFinal[] = $arrCondicaoDentroGrupoHaving[$n-1]." coalesce(mun.munpopulacao,0) between ".$arrValorInicialHaving[$n-1]." and ".$arrValorFinalHaving[$n-1];
									}else{
										if($arrValorInicialHaving[$n-1] != "true" && $arrValorInicialHaving[$n-1] != "false"){
											$arrHavingFinal[] = $arrCondicaoDentroGrupoHaving[$n-1]." coalesce(mun.munpopulacao,0) ".$arrCondicaoHaving[$n-1]." ".$arrValorInicialHaving[$n-1];
										}
									}
									//$arrGroupBy[] = "mun.munpopulacao";
								}
								$n++;
							}
							if($arrHavingFinal){
								$having_final.= " (".implode(" ",$arrHavingFinal).") ".$cmb_entregrupo[$grupo+1];
							}
							unset($arrHavingFinal);
						}
					}
				}
								
				/* SQL Muncod Interno */
				$sqlInterno = "select
					mun.muncod
				from 
					territoriosgeo.municipio mun
				".($arrInnerTemaMuncod ? " left join ".implode(" left join ",$arrInnerTemaMuncod) : "")."
				where $having_final ".($arrWhereUF ? $arrWhereUF :"") ."  
				";
				
				$arrInnerTema = array_unique($arrInnerTema);
				$sqlNovo = "
							select 
								tpm.tpmdsc as tipo,
								count(mun.muncod) || ' município(s)' as qtde_mun
								".($arrCampos ? ",".implode(",",str_replace("mun.munpopulacao","sum(mun.munpopulacao)",$arrCampos))."" : "")."
							from 
							territoriosgeo.municipio mun
							inner join
							territoriosgeo.muntipomunicipio mtm ON mtm.muncod = mun.muncod
							inner join
							territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid 
							".($arrInnerTema ? " left join ".implode(" left join ",$arrInnerTema) : "")."
							where mun.muncod in ( $sqlInterno ) and
							tpm.gtmid = {$_POST['agrupador']} and tpm.tpmstatus = 'A' ".($arrWhereUF ? $arrWhereUF : "")."
							group by tpm.tpmdsc,tpm.tpmid".($arrGroupBy ? ",".implode(",",$arrGroupBy) : "")."";
				//dbg($sqlNovo,1);
			}
			
	}

	$sqlNovo = str_replace(array("and (  )","(  )","( )","and and","and   
				 )"),array("","(1=1)","(1=1)","and",")"),$sqlNovo);
				
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=planilha_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$arrCabecalho = array_unique($arrCabecalho);
	$db->monta_lista_tabulado($sqlNovo,$arrCabecalho,100000,5,'N','100%',$par2);
	exit;
}

if($_POST['requisicao'] == "exibeRelatorioFiltrosAvancados"){
	unset($_POST['requisicao']);
}

$mapid = $_GET['mapid'];
extract($_POST);
	if(is_numeric($estuf)){
		$estuf = ("select estuf from territorios.estado where regcod = '".str_replace("estuf","",$estuf)."'");
		$arrWhereUF = "and mun.estuf in ($estuf)";
	}else{
		if(strstr($estuf,"tipo")){
			$arrWhereUF = "and mun.muncod in (select distinct mtm2.muncod from territoriosgeo.muntipomunicipio mtm2 where mtm2.tpmid = ".str_replace("tipo","",$estuf).")";
		}else{
			$estuf = str_replace("estuf","",$estuf);
			if($estuf){
				$arrUF = explode(",",$estuf);
				$arrWhereUF = "and mun.estuf in ('".implode("','",$arrUF)."')";
			}	
		}
	}
	?>
	<html>
	<head>
		<title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/prototype.js"></script>
	    <script type="text/javascript" src="../includes/entidades.js"></script>
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>
	    <script src="/emi/geral/js/emi.js"></script>
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
	<?php
	if($_POST['agrupador'] == "estado"){
		$arrCabecalho[0] = "<center>Estado</center>";
		$arrCabecalho[1] = "<center>Qtde Município(s)</center>";
	}elseif($_POST['agrupador'] == "regiao"){
		$arrCabecalho[0] = "<center>Região</center>";
		$arrCabecalho[1] = "<center>Qtde Município(s)</center>";
	}elseif($_POST['agrupador'] == "municipio"){
		$arrCabecalho[0] = "<center>Ação</center>";
		$arrCabecalho[1] = "<center>Município/UF</center>";
	}else{
		$sql_gt = "select gtmdsc from territoriosgeo.grupotipomunicipio where gtmid = {$_POST['agrupador']}";
		$descricao_gtm = $db->pegaUm($sql_gt);
		$arrCabecalho[0] = "<center>$descricao_gtm</center>";
		$arrCabecalho[1] = "<center>Qtde Município(s)</center>";
	}
	
	
	
	$n=1;
	$cmb_tema = !$cmb_tema ? array() : $cmb_tema;
	foreach($cmb_tema as $grupo => $arrGrupo){
		foreach($arrGrupo as $tipoFiltro => $tmaid){
			$condicaoDentroGrupo = $cmb_grupo[$grupo][$tipoFiltro];
			$condicao = $cmb_filtro[$grupo][$tipoFiltro];
			$valorCondicaoInicio = str_replace(array(".",",","%"),array("",".",""),$cmb_filtro_inicio[$grupo][$tipoFiltro]);
			$valorCondicaoFim = str_replace(array(".",",","%"),array("",".",""),$cmb_filtro_fim[$grupo][$tipoFiltro]);
			
			$sql1 = "select tmaformula,tpdid from mapa.tema where tmaid = ".$cmb_tema[$grupo][$tipoFiltro];
			$arrTma = $db->pegaLinha($sql1);
			$tpdid = $arrTma['tpdid'];
			$tmaformula = $arrTma['tmaformula'];
			if($tpdid == 6){
				include_once APPRAIZ . 'www/painel/_funcoes_formula_tema.php';
				if($_POST['agrupador'] != "municipio"){
					if($_POST['agrupador'] == "regiao"){
						$formula_coluna = retornaFormulaPopUp($tmaformula,"regiao");
					}elseif($_POST['agrupador'] == "estado"){
						$formula_coluna = retornaFormulaPopUp($tmaformula,"estado");
					}else{
						$formula_coluna = retornaFormulaPopUp($tmaformula,"tpm");
					}
					$arrHaving[] = $formula_coluna['coluna'];
					$arrCampos[] = $formula_coluna['coluna']."  as coluna{$n} ";
					if($formula_coluna['leftjoin']){
						foreach($formula_coluna['leftjoin'] as $left){
							$arrInnerTema[] = $left;
						}
					}
					$arrWhereFormula[$n] = true;
				}
				if($_POST['agrupador'] == "municipio"){
					$tpdid = atualizaDadosTemaFormula($cmb_tema[$grupo][$tipoFiltro],$_POST['agrupador']);
				}
			}
			
			$arrCondicaoDentroGrupoHaving[] = $condicaoDentroGrupo;
			$arrCondicaoHaving[] = $condicao;
			$arrValorInicialHaving[] = $valorCondicaoInicio;
			$arrValorFinalHaving[] = $valorCondicaoFim;
			
			if($tpdid == 3){
				$arrInnerTema[] = "mapa.temadado  tem_{$n} ON tem_{$n}.muncod = mun.muncod ".($tmaid == "1" ? "" : "and tem_{$n}.tmaid = $tmaid")." $arrWhereUF";
				if($_POST['agrupador'] != "municipio"){
					$arrCampos[] = "'<center>-</center>' as coluna{$n}";
				}else{
					$arrCampos[] = "'<div style=\'width:100%;color:blue;text-align:right\' >' || (CASE WHEN tem_{$n}.tmdboleano is true THEN 'Sim' ELSE 'Não' END) || '</div>' as coluna{$n}";
				}
				$arrWhere[$grupo][] = "$condicaoDentroGrupo COALESCE(tem_{$n}.tmdboleano,false) is $condicao";
				$tmdsc = $db->pegaUm("select tmadsc from mapa.tema where tmaid = $tmaid");
				if($condicao == "true"){
					switch($condicaoDentroGrupo){
						case "and":
						$condicao_extenso = "e";
						break;
						case "or":
						$condicao_extenso = "ou";
						break;
						default:
						$condicao_extenso = "";
						break;
					}
					$arrFormula[$grupo][] = "<b>$condicao_extenso</b> $tmdsc ";
					$arrCabecalho[1+(int)$grupo]["colunas"][] = "<center>$condicao_extenso $tmdsc <br /></center>";
				}else{
					switch($condicaoDentroGrupo){
						case "and":
						$condicao_extenso = "e";
						break;
						case "or":
						$condicao_extenso = "ou";
						break;
						default:
						$condicao_extenso = "";
						break;
					}
					$arrFormula[$grupo][] = "<b>$condicao_extenso</b> Não $tmdsc ";
					$arrCabecalho[1+(int)$grupo]["colunas"][] = "<center>$condicao_extenso Não $tmdsc <br /></center>";	
				}
				if($tipoFiltro == 1){
					$condicaoGrupo = str_replace(array("and","or"),array("e ","ou "),$cmb_entregrupo[$grupo]);
					$arrCabecalho[1+(int)$grupo]["label"] = "$condicaoGrupo Grupo $grupo";
				}
			}else{
				if($tmaid != "1"){
					$arrInnerTema[] = "mapa.temadado  tem_{$n} ON tem_{$n}.muncod = mun.muncod ".($tmaid == "1" ? "" : "and tem_{$n}.tmaid = $tmaid")." $arrWhereUF";
					$arrInnerTemaMuncod[] = "mapa.temadado  tem_{$n} ON tem_{$n}.muncod = mun.muncod ".($tmaid == "1" ? "" : "and tem_{$n}.tmaid = $tmaid")." $arrWhereUF";
					if($_POST['agrupador'] != "municipio"){
						if($tpdid == 4){
							$arrCamposMuncod[] = "COALESCE(tem_{$n}.tmdvalor,0) as coluna{$n}";
						}else{
							$arrCamposMuncod[] = "COALESCE(tem_{$n}.tmdvalor,0)::integer as coluna{$n}";
						}
						if($tpdid == 2 || $tpdid == 1){
							$arrCampos[] = "sum(COALESCE(tem_{$n}.tmdvalor,0))::integer as coluna{$n}";
							$arrHaving[] = "sum(COALESCE(tem_{$n}.tmdvalor,0))::integer";
						}else{
							if(!$formula_coluna){
								$arrCampos[] = "'<center>-</center>' as coluna{$n}";
							}else{
								$formula_coluna = false;
							}
						}
					}else{
						if($tpdid == 4){
							$arrCampos[] = "COALESCE(tem_{$n}.tmdvalor,0) as coluna{$n}";
						}else{
							$arrCampos[] = "COALESCE(tem_{$n}.tmdvalor,0)::integer as coluna{$n}";
						}	
					}
					if($condicao == "entre"){
						if( !$_POST['agrupador'] || ($_POST['agrupador'] != "municipio" && ($tpdid == 2 || $tpdid == 1)) ){
							$arrWhereExterno[$grupo][] = "$condicaoDentroGrupo coluna{$n} between $valorCondicaoInicio and $valorCondicaoFim";
						}
						$arrWhere[$grupo][] = "$condicaoDentroGrupo COALESCE(tem_{$n}.tmdvalor,0) between $valorCondicaoInicio and $valorCondicaoFim";
					}else{
						if( !$_POST['agrupador'] || ($_POST['agrupador'] != "municipio" && ($tpdid == 2 || $tpdid == 1)) ){
							$arrWhereExterno[$grupo][] = "$condicaoDentroGrupo coluna{$n} $condicao $valorCondicaoInicio";
						}
						$arrWhere[$grupo][] = "$condicaoDentroGrupo COALESCE(tem_{$n}.tmdvalor,0) $condicao $valorCondicaoInicio";
					}
				}else{
					$arrCampos[] = "mun.munpopulacao as coluna{$n}";
					if($condicao == "entre"){
						$arrWhere[$grupo][] = "$condicaoDentroGrupo mun.munpopulacao between $valorCondicaoInicio and $valorCondicaoFim";
					}else{
						$arrWhere[$grupo][] = "$condicaoDentroGrupo mun.munpopulacao $condicao $valorCondicaoInicio";
					}
				}	
				
				$tmdsc = $db->pegaUm("select tmadsc from mapa.tema where tmaid = $tmaid");
				
				if($condicao == "entre"){
					switch($condicaoDentroGrupo){
						case "and":
						$condicao_extenso = "e";
						break;
						case "or":
						$condicao_extenso = "ou";
						break;
						default:
						$condicao_extenso = "";
						break;
					}
					$arrFormula[$grupo][] = "<b>$condicao_extenso</b> $tmdsc entre $valorCondicaoInicio e $valorCondicaoFim";
				}else{
					switch($condicaoDentroGrupo){
						case "and":
						$condicao_extenso = "e";
						break;
						case "or":
						$condicao_extenso = "ou";
						break;
						default:
						$condicao_extenso = "";
						break;
					}
					$arrFormula[$grupo][] = "<b>$condicao_extenso</b> $tmdsc $condicao $valorCondicaoInicio";
				}
	
				$arrCabecalho[1+(int)$grupo]["colunas"][] = "<center>$condicao_extenso $tmdsc <br /> $condicao $valorCondicaoInicio</center>";
				if($tipoFiltro == 1){
					$condicaoGrupo = str_replace(array("and","or"),array("e ","ou "),$cmb_entregrupo[$grupo]);
					$arrCabecalho[1+(int)$grupo]["label"] = "$condicaoGrupo Grupo $grupo";
				}
			}
			if($tpdid != 4 && $tpdid != 3){
				$arrTemasTotal["coluna".$n] = $tmdsc;
				$arrTipoTotal["coluna".$n] = $tpdid;
			}
			$n++;
		}
	}
	$arrWhere = !$arrWhere ? array(): $arrWhere; 
	foreach($arrWhere as $grupo => $arrGrupo){
		if($cmb_entregrupo[$grupo]){
			if($arrWhereExterno[$grupo]){
				$sqlExterno.= " ) ".$cmb_entregrupo[$grupo]." (";	
			}else{
				$sql.= " ) ".$cmb_entregrupo[$grupo]." (";	
			}
			switch($cmb_entregrupo[$grupo]){
				case "and":
				$condicao_extenso = "e";
				break;
				case "or":
				$condicao_extenso = "ou";
				break;
			}
			$formula.= " ) <br /> <b>".$condicao_extenso."</b> <br /> <b>Grupo $grupo:</b> (";
		}
		if($arrWhereExterno[$grupo]){
			foreach($arrWhereExterno[$grupo] as $whereExterno){
				$sqlExterno.= " ".$whereExterno." ";
			}	
		}else{
			foreach($arrWhere[$grupo] as $where){
				$sql.= " ".$where." ";
			}	
		}
		foreach($arrFormula[$grupo] as $form){
			$formula.= " ".$form." ";
		}
	}
	$sql = "( $sql )";
	$sql = str_replace(array("and (  )","(  )","( )"),array("","(1=1)","(1=1)"),$sql);
	$sqlExterno = "( $sqlExterno )";
	$sqlExterno = str_replace(array("(  )","( )"),array("(1=1)","(1=1)"),$sqlExterno);
	$sql = str_replace("select u.usuchaveativacao from seguranca.usuario u where u.usucpf = '{$_SESSION['usucpf']}'","",$sql);
	monta_titulo("Municípios", "<font size=2 ><b>Fórmula:</b></font><br /><b>Grupo 1:</b> ( $formula )<br /><br /><a href=\"javascript:exportaExcel()\" ><img style=\"border:0px;vertical-align:middle\" src=\"../imagens/excel.gif\"  /> Exportar para Planilha</a>" );
	
	?>
	<script>
		function mudaAgrupador(agp)
		{
			if(document.formlista){
				document.formlista.excel.value = "";
				if(agp == "estado"){
					//document.formlista.estuf.value = "";
				}
				if(agp == "regiao"){
					//document.formlista.estuf.value = "";
				}
				document.formlista.agrupador.value = agp;
				document.getElementById('div_aguarde').innerHTML = 'Aguarde...Carregando.'
				document.formlista.submit();
			}else{
				document.form2.excel.value = "";
				if(agp == "estado"){
					//document.form2.estuf.value = "";
				}
				if(agp == "regiao"){
					//document.form2.estuf.value = "";
				}
				document.form2.agrupador.value = agp;
				document.getElementById('div_aguarde').innerHTML = 'Aguarde...Carregando.'
				document.form2.submit();
			}
		}
		function filtraEstado(estuf)
		{
			document.formlista.estuf.value = estuf;
			document.formlista.agrupador.value = 'municipio';
			document.formlista.excel.value = "";
			document.formlista.submit();
		}
		function filtraRegiao(regcod)
		{
			document.formlista.estuf.value = regcod;
			document.formlista.agrupador.value = '';
			document.formlista.excel.value = "";
			document.formlista.submit();
		}
		function filtraTipoMunicipio(tpmid)
		{
			document.formlista.estuf.value = 'tipo'+tpmid;
			document.formlista.agrupador.value = '';
			document.formlista.excel.value = "";
			document.formlista.submit();
		}
		function exportaExcel()
		{
			document.formlista.excel.value = "excel";
			document.formlista.submit();
		}
	</script>
	<table width="100%" align="center"  class="tabela" >
		<tr>
			<td width="25%" class="SubtituloDireita" ><b>Agrupador:</b></td>
			<td id="div_aguarde" >
				<?php

				$_GET['mapid'] = $_GET['mapid'] ? $_GET['mapid'] : $_SESSION['painel_vars']['mapid'];
				$grupotm = $db->carregar("	SELECT 
												gtm.gtmid as codigo,
												gtmdsc as descricao 
											FROM 
												territoriosgeo.grupotipomunicipio gtm
											INNER JOIN
												mapa.mapacamada mpc ON mpc.gtmid = gtm.gtmid 
											WHERE 
												mpc.mapid = {$_GET['mapid']}
											/*and
												gtm.gtmid not in (8,10)*/
											AND
												gtmstatus='A' 
											ORDER BY 
												gtmdsc");
				if(!$grupotm){
					$grupotm = $db->carregar("	SELECT 
												gtm.gtmid as codigo,
												gtmdsc as descricao 
											FROM 
												territoriosgeo.grupotipomunicipio gtm
											WHERE 
												gtmstatus='A'
											/*and
												gtm.gtmid not in (8,10)*/ 
											ORDER BY 
												gtmdsc");
				}
				$arrAgrupadores = array(
										0 => array( "codigo" => "municipio" , "descricao" => "Município" ),
										/*1 => array( "codigo" => "estado" , "descricao" => "Estado" ),
										2 => array( "codigo" => "regiao" , "descricao" => "Região" )*/
				);
				if($grupotm[0]) {
					foreach($grupotm as $gtm) {
						$arrAgrupadores[] = $gtm;
					}
				}
				
				$db->monta_combo("cnb_agrupador",$arrAgrupadores,"S",false,"mudaAgrupador","","","","","cnb_agrupador","",$_POST['agrupador']); 
				?>
			</td>
		</tr>
	</table>
	<?php
	
	//Agrupadores
	switch($_POST['agrupador']){
		
		case "estado":
			
			if($camada != "10"){
				if($arrCampos){
					$n=1;
					if($arrWhere[1]){
						foreach($arrWhere[1] as $where){
							if(!$arrWhereFormula[$n]){
								$where_interno_agrupador .= " and ".$arrWhere[1][$n-1];
								$where_interno_agrupador = str_replace( array("sum(COALESCE(" , "))"), array(" "," ") , $where_interno_agrupador);
							}else{
								$arrWhereExternoFormula[] = str_replace("tem_$n.tmdvalor","coluna$n",$arrWhere[1][$n-1]);
							}
							$n++;
						}
					}
				}
				$sqlNovo = "select * from ( select distinct
					'<a href=\"javascript:filtraEstado(\'' || est.estuf || '\')\" >' || est.estdescricao || '</a>' as estado,
					count( distinct mun.muncod) || ' município(s)' as qtde_mun
					".($arrCampos ? ",".implode(",",$arrCampos)."" : "")."
				from 
					territoriosgeo.municipio mun
				inner join
					territoriosgeo.estado est ON est.estuf = mun.estuf
				".($arrInnerTema ? " left join ".implode(" left join ",$arrInnerTema) : "")."
				where ".($arrWhereUF ? " 1=1 ".$arrWhereUF." " : "")."
				";
								
				$sqlNovo = str_replace( array( "mun.munpopulacao","COALESCE(" , ",0)" ) , array( "sum(mun.munpopulacao)","sum(COALESCE(" , ",0))" ) , $sqlNovo);
				
				$sqlNovo.= "$where_interno_agrupador";
				
				$sqlNovo .= " group by est.estdescricao,est.estuf";
				
				if(!strstr(strtoupper($sqlNovo),"ORDER BY")){
					//$sqlNovo .= " order by estado";
				}
				$sqlNovo .= ") as foo where 1=1 ".($arrWhereExternoFormula ? " and ".implode(" ",$arrWhereExternoFormula) : "")." ";
			}else{
				$sqlNovo = "select * from ( select distinct
					'<a href=\"javascript:filtraEstado(\'' || est.estuf || '\')\" >' || est.estdescricao || '</a>' as estado,
					count( distinct mun.muncod) || ' município(s)' as qtde_mun
					".($arrCampos ? ",".implode(",",$arrCampos)."" : "")."
				from 
					territoriosgeo.municipio mun
				inner join
					territoriosgeo.estado est ON est.estuf = mun.estuf
				".($arrInnerTema ? " left join ".implode(" left join ",$arrInnerTema) : "")."
				where
				";
				
				$sqlNovo = str_replace( array( "mun.munpopulacao","COALESCE(" , ",0)" ) , array( "sum(mun.munpopulacao)","sum(COALESCE(" , ",0))" ) , $sqlNovo);
				
				$sqlNovo .= $sql . " group by est.estdescricao,est.estuf";
				
				if(!strstr(strtoupper($sqlNovo),"ORDER BY")){
					//$sqlNovo .= " order by estado";
				}
				$sqlNovo .= ") as foo where $sqlExterno";	
			}
			break;
		case "regiao":
			if($camada != "8"){
				if($arrCampos){
					$n=1;
					if($arrWhere[1]){
						foreach($arrWhere[1] as $where){
							if(!$arrWhereFormula[$n]){
								$where_interno_agrupador .= " and ".$arrWhere[1][$n-1];
								$where_interno_agrupador = str_replace( array("sum(COALESCE(" , "))"), array(" "," ") , $where_interno_agrupador);
							}else{
								$arrWhereExternoFormula[] = str_replace("tem_$n.tmdvalor","coluna$n",$arrWhere[1][$n-1]);
							}
							$n++;
						}
					}
				}
				
				$sqlNovo = "select * from ( select distinct
					'<a href=\"javascript:filtraRegiao(\'' || reg.regcod || '\')\" >' || reg.regdescricao || '</a>' as regiao,
					count(distinct mun.muncod) || ' município(s)' as qtde_mun
					".($arrCampos ? ",".implode(",",$arrCampos)."" : "")."
				from 
					territoriosgeo.municipio mun
				inner join
					territoriosgeo.estado est ON est.estuf = mun.estuf
				inner join
					territorios.regiao reg ON reg.regcod = est.regcod
				".($arrInnerTema ? " left join ".implode(" left join ",$arrInnerTema) : "")."
				where ".($arrWhereUF ? " 1=1 ".$arrWhereUF." " : "")."
				";
				
				$sqlNovo = str_replace( array( "mun.munpopulacao","COALESCE(" , ",0)" ) , array( "sum(mun.munpopulacao)","sum(COALESCE(" , ",0))" ) , $sqlNovo);
				$sqlNovo .="$where_interno_agrupador";
				$sqlNovo .= " group by reg.regdescricao,reg.regcod";
				
				if(!strstr(strtoupper($sqlNovo),"ORDER BY")){
					//$sqlNovo .= " order by regiao";
				}
				$sqlNovo .= ") as foo where 1=1 ".($arrWhereExternoFormula ? " and ".implode(" ",$arrWhereExternoFormula) : "")." ";
			}else{
				$sqlNovo = "select * from ( select distinct
					'<a href=\"javascript:filtraRegiao(\'' || reg.regcod || '\')\" >' || reg.regdescricao || '</a>' as regiao,
					count(distinct mun.muncod) || ' município(s)' as qtde_mun
					".($arrCampos ? ",".implode(",",$arrCampos)."" : "")."
				from 
					territoriosgeo.municipio mun
				inner join
					territoriosgeo.estado est ON est.estuf = mun.estuf
				inner join
					territorios.regiao reg ON reg.regcod = est.regcod
				".($arrInnerTema ? " left join ".implode(" left join ",$arrInnerTema) : "")."
				where ".($arrWhereUF ? $arrWhereUF." and " : "")."
				";
				
				$sqlNovo = str_replace( array( "mun.munpopulacao","COALESCE(" , ",0)" ) , array( "sum(mun.munpopulacao)","sum(COALESCE(" , ",0))" ) , $sqlNovo);
				
				$sqlNovo .= $sql . " group by reg.regdescricao,reg.regcod";
				
				if(!strstr(strtoupper($sqlNovo),"ORDER BY")){
					//$sqlNovo .= " order by regiao";
				}
				$sqlNovo .= ") as foo where $sqlExterno";
			}
			break;
		case "municipio":
			$sqlNovo = "select * from ( select distinct
				'<span style=\"display:none\">' || mun.mundescricao || '</span><img src=\"../imagens/globo_terrestre.png\" style=\"cursor:pointer\" onclick=\"window.opener.localizaMapa2(\'' || mun.muncod || '\',' || ST_X(munlatlong) || ',' || ST_Y(munlatlong) || ')\" />',
				mun.mundescricao || '/' || mun.estuf as mundesc
				".($arrCampos ? ",".implode(",",$arrCampos) : "")."
			from 
				territoriosgeo.municipio mun
			".($arrInnerTema ? " left join ".implode(" left join ",$arrInnerTema) : "")."
			where 1=1 ".($arrWhereUF ? $arrWhereUF :"") ." and  
			$sql";
			
			if(!strstr(strtoupper($sqlNovo),"ORDER BY")){
				//$sqlNovo .= " order by mundesc";
			}
			$sqlNovo .= ") as foo where $sqlExterno";
			break;
		default:
			$camada_filtros_avancados = str_replace("tipmunici_","",$camada_filtros_avancados);
			
			if($camada_filtros_avancados != "municipio" && $camada_filtros_avancados != ""){
				if($arrCampos){
					$n=1;
					if($arrWhere){
						foreach($arrWhere as $grupo => $arrW){
							foreach($arrW as $where){
								if($arrCondicaoHaving[$n-1] == "entre"){
									$arrHavingFinal[] = $arrCondicaoDentroGrupoHaving[$n-1]." ".$arrHaving[$n-1]." between ".$arrValorInicialHaving[$n-1]." and ".$arrValorFinalHaving[$n-1];
								}else{
									$arrHavingFinal[] = $arrCondicaoDentroGrupoHaving[$n-1]." ".$arrHaving[$n-1]." ".$arrCondicaoHaving[$n-1]." ".$arrValorInicialHaving[$n-1];
								}
								$n++;
							}
							$having_final.= " (".implode(" ",$arrHavingFinal).") ".$cmb_entregrupo[$grupo+1];
							unset($arrHavingFinal);
						}
					}
				}
				
				$arrInnerTema = array_unique($arrInnerTema);
				$sqlNovo = "
							select 
								'<span style=\"display:none\">' || tpm.tpmdsc || '</span><a href=\"javascript:filtraTipoMunicipio(\'' || tpm.tpmid || '\')\" >' || tpm.tpmdsc || '</a>' as tipo,
								count(mun.muncod) || ' município(s)' as qtde_mun
								".($arrCampos ? ",".implode(",",$arrCampos)."" : "")."
							from 
							territoriosgeo.municipio mun
							inner join
							territoriosgeo.muntipomunicipio mtm ON mtm.muncod = mun.muncod
							inner join
							territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid 
							".($arrInnerTema ? " left join ".implode(" left join ",$arrInnerTema) : "")."
							where
							tpm.gtmid = {$_POST['agrupador']} and tpm.tpmstatus = 'A' ".($arrWhereUF ? $arrWhereUF : "")."
							group by tpm.tpmdsc,tpm.tpmid
							".($having_final ? "having ".$having_final."" : "")."";
			}else{
				if($arrCampos){
					$n=1;
					if($arrWhere){
						foreach($arrWhere as $grupo => $arrW){
							foreach($arrW as $where){
								if(!strstr($where,"tmdboleano") && !strstr($where,"munpopulacao")){
									if($arrCondicaoHaving[$n-1] == "entre"){
										$arrHavingFinal[] = $arrCondicaoDentroGrupoHaving[$n-1]." coalesce(tem_".($n).".tmdvalor,0) between ".$arrValorInicialHaving[$n-1]." and ".$arrValorFinalHaving[$n-1];
									}else{
										if($arrValorInicialHaving[$n-1] != "true" && $arrValorInicialHaving[$n-1] != "false"){
											$arrHavingFinal[] = $arrCondicaoDentroGrupoHaving[$n-1]." coalesce(tem_".($n).".tmdvalor,0) ".$arrCondicaoHaving[$n-1]." ".$arrValorInicialHaving[$n-1];
										}
									}
								}
								if(strstr($where,"munpopulacao")){
									if($arrCondicaoHaving[$n-1] == "entre"){
										$arrHavingFinal[] = $arrCondicaoDentroGrupoHaving[$n-1]." coalesce(mun.munpopulacao,0) between ".$arrValorInicialHaving[$n-1]." and ".$arrValorFinalHaving[$n-1];
									}else{
										if($arrValorInicialHaving[$n-1] != "true" && $arrValorInicialHaving[$n-1] != "false"){
											$arrHavingFinal[] = $arrCondicaoDentroGrupoHaving[$n-1]." coalesce(mun.munpopulacao,0) ".$arrCondicaoHaving[$n-1]." ".$arrValorInicialHaving[$n-1];
										}
									}
									//$arrGroupBy[] = "mun.munpopulacao";
								}
								$n++;
							}
							if($arrHavingFinal){
								$having_final.= " (".implode(" ",$arrHavingFinal).") ".$cmb_entregrupo[$grupo+1];
							}
							unset($arrHavingFinal);
						}
					}
				}		
				/* SQL Muncod Interno */
				$sqlInterno = "select
					mun.muncod
				from 
					territoriosgeo.municipio mun
				".($arrInnerTemaMuncod ? " left join ".implode(" left join ",$arrInnerTemaMuncod) : "")."
				where $having_final ".($arrWhereUF ? $arrWhereUF :"") ."  
				";
				
				$arrInnerTema = array_unique($arrInnerTema);
				
				$sqlNovo = "
							select 
								'<span style=\"display:none\">' || tpm.tpmdsc || '</span><a href=\"javascript:filtraTipoMunicipio(\'' || tpm.tpmid || '\')\" >' || tpm.tpmdsc || '</a>' as tipo,
								count(mun.muncod) || ' município(s)' as qtde_mun
								".($arrCampos ? ",".implode(",",str_replace("mun.munpopulacao","sum(mun.munpopulacao)",$arrCampos))."" : "")."
							from 
							territoriosgeo.municipio mun
							inner join
							territoriosgeo.muntipomunicipio mtm ON mtm.muncod = mun.muncod
							inner join
							territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid 
							".($arrInnerTema ? " left join ".implode(" left join ",$arrInnerTema) : "")."
							where mun.muncod in ( $sqlInterno ) and
							tpm.gtmid = {$_POST['agrupador']} and tpm.tpmstatus = 'A' ".($arrWhereUF ? $arrWhereUF : "")."
							group by tpm.tpmdsc,tpm.tpmid".($arrGroupBy ? ",".implode(",",$arrGroupBy)."" : "")."";
				//dbg($sqlNovo,1);
			}
		break;
		
	}

	if($_POST['agrupador'] != $camada){
		$sqlNovo = str_replace(array("and (  )","(  )","( )","(1=1)","(  and","where 
				 and","and or","where 
				 group","and and","and   
				 )"),array("","(1=1)","(1=1)","","(","where"," or "," group","and",")"),$sqlNovo);
	}else{
		$sqlNovo = str_replace(array("and (  )","(  )","( )","and and","and   
				 )"),array("","(1=1)","(1=1)","and",")"),$sqlNovo);
	}
	//dbg(simec_htmlentities($sqlNovo),1);
	$arrDados = $db->carregar($sqlNovo);
	if($arrDados){
		foreach($arrDados as $dado){
			foreach($dado as $coluna => $valor){
				if(strstr($coluna,"coluna")){
					if( strstr(trim($valor),"Sim") || strstr(trim($valor),"Não") ){
						$valor = 1;
					}
					$arrTotal[$coluna]+= $valor;
				}
				if(strstr($valor,"município")){
					$arrTotalMunicipios+=(int)str_replace(" município(s)","",$valor);
				}
			}
		}
	}
	
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\" >";
	if($arrTotalMunicipios){
		echo "<tr><td width=\"25%\" class=\"SubtituloDireita\" >Total de Municípios:</td>";
		echo "<td>".number_format($arrTotalMunicipios,0,',','.')."</td></tr>";
	}
	if($arrTemasTotal){
		foreach($arrTemasTotal as $chave => $tema){
			if($arrTipoTotal[$tema] == 1 || $arrTipoTotal[$tema] == 4){
				$varlorFinal = number_format($arrTotal[$chave],2,',','.');
			}else{
				$varlorFinal = number_format($arrTotal[$chave],0,',','.');
			}
			
			echo "<tr><td width=\"35%\" class=\"SubtituloDireita\" >Total de $tema:</td>";
			echo "<td>".$varlorFinal."</td></tr>";
		}
	}
	echo "</table>";
	$db->monta_lista($sqlNovo,$arrCabecalho,100,10,"N","95%","N");
	?>
	<form name="form2" id="form2" method="post" >
		<?php 
			if($_POST){
				foreach($_POST as $k=>$v){
					if ($k<>'ordemlista' and $k<>'ordemlistadir' and $k<>'numero'){
						if( is_array($v)){
							foreach($v as $k2 => $v2){
								if( is_array($v2)){
									foreach($v2 as $k3 => $v3){
										print '<input type="Hidden" name="'.$k.'['.$k2.']['.$k3.']" value="'.$v3.'"/>';
									}
								}else{
									print '<input type="Hidden" name="'.$k.'['.$k2.']" value="'.$v2.'"/>';
								}
							}
						}else{
							print '<input type="Hidden" name="'.$k.'" value="'.$v.'"/>';
						}
					}
				}
			}
		 ?>
	</form>
	</body></html>
	<?php ?>