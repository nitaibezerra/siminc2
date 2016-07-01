<?php
function trataFormula($tmaformula,$agrupador = "municipio",$arrWhere = array()){
	global $db;
	$arrItens = array();
	//Procurar por parenteses
	if(strstr($tmaformula,"(")){
		$arr1 = explode("(",$tmaformula);
		foreach($arr1 as $a1){
			if(strstr($a1,")")){
				$arr2 = explode(")",$a1);
				foreach($arr2 as $a2){
					if(strstr($a2,"{")){
						$arr3 = explode("{",$a2);
						foreach($arr3 as $a3){
							if(strstr($a3,"}")){
								$fim = strpos($a3,"}");
								$item = substr($a3,0,$fim);
								if(!in_array($item,$arrItens)){
									$arrItens[] = $item;
								}
							}
						}
					}
				}
			}
		}
	}else{ //Sem parenteses
		if(strstr($tmaformula,"{")){
			$arr3 = explode("{",$tmaformula);
			foreach($arr3 as $a3){
				if(strstr($a3,"}")){
					$fim = strpos($a3,"}");
					$item = substr($a3,0,$fim);
					if(!in_array($item,$arrItens)){
						$arrItens[] = $item;
					}
				}
			}
		}
	}
	if($arrItens){
		foreach($arrItens as $i){
			if(strstr($i,"Indicador:")){
				$indicador = str_replace(array("Indicador:"," "),array("",""),$i);
				$ind = explode("_",$indicador);
				$indid  = $ind[0];
				$dpedsc = $ind[1];
				
				if($agrupador != "municipio"){
					if(is_numeric($agrupador)){
						$inner_join = " inner join
											territoriosgeo.muntipomunicipio mtm ON mtm.muncod =  dsh.dshcodmunicipio
										inner join
											territoriosgeo.tipomunicipio tpm ON mtm.tpmid = tpm.tpmid ";
						$coluna_muncod = "(select 
												muncod 
											from 
												territoriosgeo.muntipomunicipio mtm2 
											inner join 
												territoriosgeo.tipomunicipio tpm2 ON mtm2.tpmid = tpm2.tpmid 
											where 
												tpm2.tpmid = tpm.tpmid 

											limit 1)";
						$group_by = "tpm.tpmid";
						$where = " and gtmid = $agrupador ";
					}elseif($agrupador == "estado"){
						$inner_join = " inner join territoriosgeo.municipio mun ON mun.muncod= dsh.dshcodmunicipio ";
						$coluna_muncod = "(select muncod from territoriosgeo.municipio mun2 where mun.estuf = mun2.estuf limit 1)";
						$group_by = "mun.estuf";
					}elseif($agrupador == "regiao"){
						$inner_join = " inner join territoriosgeo.municipio mun ON mun.muncod=dsh.dshcodmunicipio inner join territoriosgeo.estado est ON est.estuf=mun.estuf ";
						$coluna_muncod = "(select muncod from territoriosgeo.municipio mun2 inner join territoriosgeo.estado est2 ON est2.estuf=mun2.estuf  where est2.regcod = est.regcod limit 1)";
						$group_by = "est.regcod";
					}
				}else{
					$coluna_muncod = "dsh.dshcodmunicipio";
					$group_by = "dsh.dshcodmunicipio";
				}
				
				$sql = "select 
							sum(dsh.dshqtde) as qtde,
							$coluna_muncod as muncod
						from 
							painel.indicador ind
						inner join
							painel.seriehistorica seh ON seh.indid = ind.indid
						inner join
							painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
						inner join
							painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
						$inner_join
						where 
							ind.indid = $indid 
						and 
							dpe.dpedsc = '$dpedsc'
						and
							dsh.dshcodmunicipio is not null $where
						group by
							$group_by";
				//dbg($sql);
				$arrDados[$i] = $db->carregar($sql);
				if($arrDados[$i]){
					foreach($arrDados[$i] as $a){
						$arrQtde[$i][$a['muncod']] = $a['qtde'];
					}
				}
			}
			if(strstr($i,"Tema:")){
				$tema = str_replace(array("Tema:"," "),array("",""),$i);
				if($agrupador != "municipio"){
					if(is_numeric($agrupador)){
						$coluna_qtde = "sum(tmdvalor)";
						$inner_join = " inner join
											territoriosgeo.muntipomunicipio mtm ON mtm.muncod = tmd.muncod
										inner join
											territoriosgeo.tipomunicipio tpm ON mtm.tpmid = tpm.tpmid";
						$coluna_muncod = "(select 
												muncod 
											from 
												territoriosgeo.muntipomunicipio mtm2 
											inner join 
												territoriosgeo.tipomunicipio tpm2 ON mtm2.tpmid = tpm2.tpmid 
											where 
												tpm2.tpmid = tpm.tpmid 
											limit 1)";
						$group_by = "tma.tpdid,tmaformula,tpm.tpmid";
						$where = " and gtmid = $agrupador ";
					}elseif($agrupador == "estado"){
						$inner_join = " inner join territoriosgeo.municipio mun ON mun.muncod=tmd.muncod ";
						$coluna_qtde = "sum(tmdvalor)";
						$coluna_muncod = "(select muncod from territoriosgeo.municipio mun2 where mun.estuf = mun2.estuf limit 1)";
						$group_by = "tma.tpdid,mun.estuf,tmaformula";
					}elseif($agrupador == "regiao"){
						$coluna_qtde = "sum(tmdvalor)";
						$inner_join = " inner join territoriosgeo.municipio mun ON mun.muncod=tmd.muncod inner join territoriosgeo.estado est ON est.estuf=mun.estuf ";
						$coluna_muncod = "(select muncod from territoriosgeo.municipio mun2 inner join territoriosgeo.estado est2 ON est2.estuf=mun2.estuf  where est2.regcod = est.regcod limit 1)";
						$group_by = "tma.tpdid,est.regcod,tmaformula";
					}
				}else{
					$coluna_qtde = "tmdvalor";
					$coluna_muncod = "tmd.muncod";
					$group_by = "tma.tpdid,tmdvalor,tmd.muncod,tmaformula";
				}
				
				$sql = "select 
							tma.tpdid,
							$coluna_qtde as qtde,
							$coluna_muncod as muncod,
							tmaformula 
						from 
							mapa.tema tma
						left join
							mapa.temadado tmd ON tma.tmaid = tmd.tmaid
						$inner_join 
						where 
							tma.tmaid = $tema $where
							".($arrWhere ? " ". implode(" ",$arrWhere)." " : "")."
						group by
							$group_by";
				//dbg($sql);
				$arrDados[$i] = $db->carregar($sql);
				if($arrDados[$i]){
					foreach($arrDados[$i] as $a){
						if($a['tpdid'] != 6){
							$arrQtde[$i][$a['muncod']] = $a['qtde'];
						}else{
							$arrQtde[$i] = trataFormula($a['tmaformula']);
							break;
						}
					}
				}
			}
			
		}
	}
	$sql = "select muncod from territorios.municipio";
	$arrMuncod = $db->carregarColuna($sql);
	foreach($arrMuncod as $m1){
		$arrQtde = $arrQtde ? $arrQtde : array();
		foreach($arrQtde as $chave => $arrM){
			if($arrQtde[$chave][$m1]){
				if($arrResultado[$m1]){
					$arrResultado[$m1] = str_replace("{".$chave."}",$arrQtde[$chave][$m1],$arrResultado[$m1]);
				}else{
					$arrResultado[$m1] = str_replace("{".$chave."}",$arrQtde[$chave][$m1],$tmaformula);
				}
			}
		}
	}
	if($arrResultado){
		foreach($arrResultado as $chave => $res){
			if(!strstr($res,"Tema") && !strstr($res,"Indicador")){
				$val = false;
				eval('$val = '.$res.';');
				$arrResultadoFinal[$chave] = $val;
			}
		}
	}
	return $arrResultadoFinal;
}

function atualizaDadosTemaFormula($tmaid, $agrupador = "municipio")
{
	global $db;
	$sql = "select
				tpd.tpdid,
				tpdcampotema,
				tmaformula,
				tmacor,
				tpdidformula
			from
				 mapa.tema tma
			inner join
				mapa.tipodado tpd ON tpd.tpdid = tma.tpdid
			where
				tmaid = $tmaid";
	$arrTpd = $db->pegaLinha($sql);
	$tmaformula = $arrTpd['tmaformula'];
	$tpdidformula = $arrTpd['tpdidformula'];
	$arrResultadoFinal = trataFormula($tmaformula,$agrupador);
	$sqlI = "delete from  mapa.temadado where tmaid = $tmaid;";
	if($arrResultadoFinal){
		foreach($arrResultadoFinal as $muncod => $valor){
			$sqlI .= "insert into mapa.temadado (tmaid,tmdvalor,muncod,tmdboleano,tmdtexto) values ($tmaid,'$valor','$muncod',null,null);";
		}
	}
	$db->executar($sqlI);
	$db->commit();
	return $tpdidformula ? $tpdidformula : "2";
}

function retornaFormulaPopUp($tmaformula,$alias){
	global $db;
	$arrItens = array();
	//Procurar por parenteses
	if(strstr($tmaformula,"(")){
		$arr1 = explode("(",$tmaformula);
		foreach($arr1 as $a1){
			if(strstr($a1,")")){
				$arr2 = explode(")",$a1);
				foreach($arr2 as $a2){
					if(strstr($a2,"{")){
						$arr3 = explode("{",$a2);
						foreach($arr3 as $a3){
							if(strstr($a3,"}")){
								$fim = strpos($a3,"}");
								$item = substr($a3,0,$fim);
								if(!in_array($item,$arrItens)){
									$arrItens[] = $item;
								}
							}
						}
					}
				}
			}
		}
	}else{ //Sem parenteses
		if(strstr($tmaformula,"{")){
			$arr3 = explode("{",$tmaformula);
			foreach($arr3 as $a3){
				if(strstr($a3,"}")){
					$fim = strpos($a3,"}");
					$item = substr($a3,0,$fim);
					if(!in_array($item,$arrItens)){
						$arrItens[] = $item;
					}
				}
			}
		}
	}
	if($arrItens){
		foreach($arrItens as $n => $item){
			if($alias == "regiao"){
				if(strstr($item,"Tema")){
					$tmaid = str_replace(array("Tema:"," "),array("",""),$item);
					$coluna[] = "(sum(agr_{$n}_{$tmaid}.tmdvalor))";
					$left[] = "mapa.temadado as agr_{$n}_{$tmaid} ON mtm.muncod = agr_{$n}_{$tmaid}.muncod AND agr_{$n}_{$tmaid}.tmaid = $tmaid";
					//$sql[] = "(select sum(agr_{$n}_{$tmaid}.tmdvalor) from mapa.temadado as agr_{$n}_{$tmaid} where agr_{$n}_{$tmaid}.tmaid = $tmaid and agr_{$n}_{$tmaid}.muncod in ( select mun_{$n}_{$tmaid}.muncod from territoriosgeo.municipio mun_{$n}_{$tmaid} inner join territoriosgeo.estado est_{$n}_{$tmaid} ON mun_{$n}_{$tmaid}.estuf = est_{$n}_{$tmaid}.estuf where est_{$n}_{$tmaid}.regcod = reg.regcod ))";
				}elseif(strstr($item,"Indicador")){
					dbg("Indicador");
				}
			}elseif($alias == "estado"){
				if(strstr($item,"Tema")){
					$tmaid = str_replace(array("Tema:"," "),array("",""),$item);
					$coluna[$tmaid] = "(sum(agr_{$n}_{$tmaid}.tmdvalor))";
					$left[] = "mapa.temadado as agr_{$n}_{$tmaid} ON mtm.muncod = agr_{$n}_{$tmaid}.muncod AND agr_{$n}_{$tmaid}.tmaid = $tmaid";
					//$sql[] = "(select sum(agr_{$n}_{$tmaid}.tmdvalor) from mapa.temadado as agr_{$n}_{$tmaid} where agr_{$n}_{$tmaid}.tmaid = $tmaid and agr_{$n}_{$tmaid}.muncod in ( select mun_{$n}_{$tmaid}.muncod from territoriosgeo.municipio mun_{$n}_{$tmaid} inner join territoriosgeo.estado est_{$n}_{$tmaid} ON mun_{$n}_{$tmaid}.estuf = est_{$n}_{$tmaid}.estuf where est_{$n}_{$tmaid}.estuf = est.estuf))";
				}elseif(strstr($item,"Indicador")){
					dbg("Indicador");
				}
			}else{
				if(strstr($item,"Tema")){
					$tmaid = str_replace(array("Tema:"," "),array("",""),$item);
					$coluna[$tmaid] = "(sum(agr_{$n}_{$tmaid}.tmdvalor))";
					$left[] = "mapa.temadado as agr_{$n}_{$tmaid} ON mtm.muncod = agr_{$n}_{$tmaid}.muncod AND agr_{$n}_{$tmaid}.tmaid = $tmaid";
					//$sql[] = "(select sum(agr_{$n}_{$tmaid}.tmdvalor) from mapa.temadado as agr_{$n}_{$tmaid} where agr_{$n}_{$tmaid}.tmaid = $tmaid and agr_{$n}_{$tmaid}.muncod in ( select distinct mun_{$n}_{$tmaid}.muncod from territoriosgeo.muntipomunicipio as mun_{$n}_{$tmaid} where mun_{$n}_{$tmaid}.tpmid = $alias.tpmid) )";
				}elseif(strstr($item,"Indicador")){
					dbg("Indicador");
				}
			}
			$n++;
		}
	}
	if($coluna){
		$formula = $tmaformula;
		foreach($coluna as $tmaid => $c){
			$formula = str_replace(array("{Tema: $tmaid}"),array("$c"),$formula);
		}
	}
	//$tmaformula = str_replace($arrItens,$sql,$tmaformula);
	//return str_replace(array("{","}"),array("",""),$tmaformula);
	return array("coluna" => $formula, "leftjoin" => $left);
}