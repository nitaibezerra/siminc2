<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

session_start();

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select
				empenho,
				codigo,
				processo,
			    notaempenho,
			    sequencial,
			    especie,
				vrlempenho,
			    vrlempcomposicao,
				vrlcancelado,
			    tipo
			from(
			    select
			    	coalesce(e.empvalorempenho, 0) as vrlempenho,
			        coalesce(sum(es.eobvalorempenho), 0) as vrlempcomposicao,
					coalesce(ep.vrlcancelado, 0) as vrlcancelado,
					pp.prpid as codigo,
			        e.empid as empenho,
			        e.empnumeroprocesso as processo,
			        e.empnumero as notaempenho,
			        e.empprotocolo as sequencial,
			        e.empcodigoespecie as especie,
			        'PAR' as tipo
			    from
			        par.processopar pp
			        inner join par.empenho e on e.empnumeroprocesso = pp.prpnumeroprocesso
			        left join par.empenhosubacao es on es.empid = e.empid and es.eobstatus = 'A'
					left join (
                    		select empnumeroprocesso, empid, empidpai, sum(empvalorempenho) as vrlcancelado, empcodigoespecie 
                            from par.empenho
                            where empcodigoespecie in ('03', '13', '04') and empstatus = 'A'
                            group by 
                            	empnumeroprocesso,
                                empcodigoespecie,
                                empidpai, empid
                    ) as ep on ep.empidpai = e.empid
			    where
			        pp.prpstatus = 'A'
			        and e.empstatus = 'A'
			        and e.empsituacao <> 'CANCELADO'
			    group by
			        e.empid,
			        e.empnumeroprocesso,
			        e.empnumero,
			        e.empcodigoespecie,
			        e.empprotocolo,
			        e.empvalorempenho,
			        e.empvalorempenho,
					pp.prpid, ep.vrlcancelado
			        
			    UNION ALL
			
			    select 
			        coalesce(e.empvalorempenho, 0) as vrlempenho,
			        coalesce(sum(es.eobvalorempenho), 0) as vrlempcomposicao,
					coalesce(ep.vrlcancelado, 0) as vrlcancelado,
					pp.proid as codigo,
			        e.empid as empenho,
			        e.empnumeroprocesso as processo,
			        e.empnumero as notaempenho,
			        e.empprotocolo as sequencial,
			        e.empcodigoespecie as especie,
			        'OBRA' as tipo
			    from
			        par.processoobraspar pp
			        inner join par.empenho e on e.empnumeroprocesso = pp.pronumeroprocesso
			        left join par.empenhoobrapar es on es.empid = e.empid and es.eobstatus = 'A'
					left join (
                    		select empnumeroprocesso, empid, empidpai, sum(empvalorempenho) as vrlcancelado, empcodigoespecie 
                            from par.empenho
                            where empcodigoespecie in ('03', '13', '04') and empstatus = 'A'
                            group by 
                            	empnumeroprocesso,
                                empcodigoespecie,
                                empidpai, empid
                    ) as ep on ep.empidpai = e.empid
			    where
			        pp.prostatus = 'A'
			        and e.empstatus = 'A'
			        and e.empsituacao <> 'CANCELADO'
			    group by
			        e.empid,
			        e.empnumeroprocesso,
			        e.empnumero,
			        e.empcodigoespecie,
			        e.empprotocolo,
			        e.empvalorempenho,
					pp.proid, ep.vrlcancelado
			        
			    UNION ALL
			
			    select 
			        coalesce(e.empvalorempenho, 0) as vrlempenho,
			        coalesce(sum(es.eobvalorempenho), 0) as vrlempcomposicao,
					coalesce(ep.vrlcancelado, 0) as vrlcancelado,
					pp.proid as codigo,
			        e.empid as empenho,
			        e.empnumeroprocesso as processo,
			        e.empnumero as notaempenho,
			        e.empprotocolo as sequencial,
			        e.empcodigoespecie as especie,
			        'PAC' as tipo
			    from
			        par.processoobra pp
			        inner join par.empenho e on e.empnumeroprocesso = pp.pronumeroprocesso
			        left join par.empenhoobra es on es.empid = e.empid and es.eobstatus = 'A'
					left join (
                    		select empnumeroprocesso, empid, empidpai, sum(empvalorempenho) as vrlcancelado, empcodigoespecie 
                            from par.empenho
                            where empcodigoespecie in ('03', '13', '04') and empstatus = 'A'
                            group by 
                            	empnumeroprocesso,
                                empcodigoespecie,
                                empidpai, empid
                    ) as ep on ep.empidpai = e.empid
			    where
			        pp.prostatus = 'A'
			        and e.empstatus = 'A'
			        and e.empsituacao <> 'CANCELADO'
			    group by
			        e.empid,
			        e.empnumeroprocesso,
			        e.empnumero,
			        e.empcodigoespecie,
			        e.empprotocolo,
			        e.empvalorempenho,
					pp.proid, ep.vrlcancelado
			) as foo
			where
				vrlempenho > vrlempcomposicao
                and processo in (select empnumeroprocesso from par.empenho where empcodigoespecie = '03' )
                --and processo in ('23400004744201238')
                and especie = '01'
                and vrlempcomposicao = 0
            order by processo";

$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

foreach ($arrDados as $v) {
	$tipo 			= $v['tipo'];
	$empenho 		= $v['empenho'];
	$codigo 		= $v['codigo'];
	
	$sql = "select empnumeroprocesso, empid, empidpai, sum(empvalorempenho) as vrlcancelado, empcodigoespecie
				from par.empenho
                where 
					empcodigoespecie in ('03', '13', '04') 
					and empstatus = 'A'
					and empidpai = {$empenho}
				group by 
                	empnumeroprocesso,
                    empcodigoespecie,
                    empidpai, empid";
	$arEmpenho = $db->carregar($sql);
	$arEmpenho = $arEmpenho ? $arEmpenho : array();
	
	if( $tipo == 'PAR' )
	{
		
		/* $sql = "select sbdid, sbdano, vrlsubacao, prpid, sbaid
				from(
				select p.sbdid, sd.sbaid, sd.sbdano, par.recuperavalorvalidadossubacaoporano(sd.sbaid, sd.sbdano) as vrlsubacao, p.prpid
								from par.processoparcomposicao p
									inner join par.subacaodetalhe sd on sd.sbdid = p.sbdid
								where
									p.ppcstatus = 'A'
				) as foo
				where
					prpid = {$codigo}
				    and vrlsubacao >= {$v['vrlempenho']}";
		$arSubacao = $db->pegaLinha($sql);
		
		if( !empty($arSubacao) ){
			if( !empty($arEmpenho[0]) ){
				#Insere informações do empenho FILHO
				foreach ($arEmpenho as $emp){
					$empidFilho = $emp['empid'];
					
					$total = $db->pegaUm("select count(eobid) from par.empenhosubacao where empid = $empidFilho and eobstatus = 'A'");
					
					if( (int)$total < (int)1 ){
						$sql = "INSERT INTO par.empenhosubacao(sbaid, empid, eobpercentualemp, eobvalorempenho, eobano, eobstatus) 
								VALUES ({$arSubacao['sbaid']}, $empidFilho, 0, {$v['vrlempenho']}, {$arSubacao['sbdano']}, 'A')";
						$db->executar($sql);
					}
				}
			}			
			#Insere informações do empenho pai
			$total = $db->pegaUm("select count(eobid) from par.empenhosubacao where empid = $empenho and eobstatus = 'A'");
					
			if( (int)$total < (int)1 ){
				$sql = "INSERT INTO par.empenhosubacao(sbaid, empid, eobpercentualemp, eobvalorempenho, eobano, eobstatus) 
						VALUES ({$arSubacao['sbaid']}, $empenho, 0, {$v['vrlempenho']}, {$arSubacao['sbdano']}, 'A')";
				$db->executar($sql);
			}
		} */
	}
	elseif( $tipo == 'OBRA' )
	{
		$sql = "select p.preid, cast(o.prevalorobra as numeric(20,2)) as vrlobra
				from par.processoobrasparcomposicao p
					inner join obras.preobra o on o.preid = p.preid
				where
					p.pocstatus = 'A'
				    and p.proid = {$codigo}
				order by o.prevalorobra desc";
		$arObra = $db->carregar($sql);
		$arObra = $arObra ? $arObra : array();
		
		if( !empty($arObra[0]['preid']) ){
			if( !empty($arEmpenho[0]) ){
				/**Insere informações do empenho FILHO*/
				foreach ($arEmpenho as $emp){
					$empidFilho = $emp['empid'];
					
					$total = $db->pegaUm("select count(eobid) from par.empenhoobrapar where empid = $empidFilho and eobstatus = 'A'");
					
					if( (int)$total < (int)1 ){
						$vrlempenho = 0;
						$boPara = false;
						foreach ($arObra as $obra) {
							$preid = $obra['preid'];
							$vrlObra = $obra['vrlobra'];
							
							$vrlempenho = (float)$vrlempenho + (float)$obra['vrlobra'];
							
							if( (float)$vrlempenho >= (float)$v['vrlempenho'] ){
								$vrlObra = ((float)$v['vrlempenho'] - ((float)$vrlempenho - (float)$obra['vrlobra']) );
								$boPara = true;
							}							
							
							$sql = "INSERT INTO par.empenhoobrapar(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp, eobstatus) 
									VALUES ($preid, $empidFilho, 0, {$vrlObra}, 0, 'A')";
							$db->executar($sql);
							
							if( $boPara == true ){
								break;
							}
						}
					}
				}
			}
			/**Insere informações do empenho pai*/
			$total = $db->pegaUm("select count(eobid) from par.empenhoobrapar where empid = $empenho and eobstatus = 'A'");
			
			if( (int)$total < (int)1 ){
				$vrlempenho = 0;
				$boPara = false;
				foreach ($arObra as $obra) {
					$preid = $obra['preid'];
					$vrlObra = $obra['vrlobra'];
						
					$vrlempenho = (float)$vrlempenho + (float)$obra['vrlobra'];
						
					if( (float)$vrlempenho >= (float)$v['vrlempenho'] ){
						$vrlObra = ((float)$v['vrlempenho'] - ((float)$vrlempenho - (float)$obra['vrlobra']) );
						$boPara = true;
					}
					$sql = "INSERT INTO par.empenhoobrapar(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp, eobstatus)
							VALUES ($preid, $empenho, 0, {$obra['vrlobra']}, 0, 'A')";
					$db->executar($sql);
					
					if( $boPara == true ){
						break;
					}
				}
			}
		}
	} 
	elseif( $tipo == 'PAC' )
	{		
		$sql = "select p.preid, cast(o.prevalorobra as numeric(20,2)) as vrlobra
				from par.processoobraspaccomposicao p
					inner join obras.preobra o on o.preid = p.preid
				where
					p.pocstatus = 'A'
				    and p.proid = {$codigo}
                order by o.prevalorobra desc";
		$arObra = $db->carregar($sql);
		$arObra = $arObra ? $arObra : array();
		
		if( !empty($arObra[0]['preid']) ){
			if( !empty($arEmpenho[0]) ){
				/**Insere informações do empenho FILHO*/
				foreach ($arEmpenho as $emp){
					$empidFilho = $emp['empid'];
					
					$total = $db->pegaUm("select count(eobid) from par.empenhoobra where empid = $empidFilho and eobstatus = 'A'");
					
					if( (int)$total < (int)1 ){
						$vrlempenho = 0;
						$boPara = false;
						foreach ($arObra as $obra) {
							$preid = $obra['preid'];
							$vrlObra = $obra['vrlobra'];
								
							$vrlempenho = (float)$vrlempenho + (float)$obra['vrlobra'];
								
							if( (float)$vrlempenho >= (float)$v['vrlempenho'] ){
								$vrlObra = ((float)$v['vrlempenho'] - ((float)$vrlempenho - (float)$obra['vrlobra']) );
								$boPara = true;
							}
							$sql = "INSERT INTO par.empenhoobra(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp, eobstatus)
									VALUES ($preid, $empidFilho, 0, {$obra['vrlobra']}, 0, 'A')";
							$db->executar($sql);
							
							if( $boPara == true ){
								break;
							}
						}
					}
				}
			}
			/**Insere informações do empenho pai*/
			$total = $db->pegaUm("select count(eobid) from par.empenhoobra where empid = $empenho and eobstatus = 'A'");	
			if( (int)$total < (int)1 ){
				$vrlempenho = 0;
				$boPara = false;
				foreach ($arObra as $obra) {
					$preid = $obra['preid'];
					$vrlObra = $obra['vrlobra'];
					
					$vrlempenho = (float)$vrlempenho + (float)$obra['vrlobra'];
					
					if( (float)$vrlempenho >= (float)$v['vrlempenho'] ){
						$vrlObra = ((float)$v['vrlempenho'] - ((float)$vrlempenho - (float)$obra['vrlobra']) );
						$boPara = true;
					}
					$sql = "INSERT INTO par.empenhoobra(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp, eobstatus)
							VALUES ($preid, $empenho, 0, {$obra['vrlobra']}, 0, 'A')";
					$db->executar($sql);
					
					if( $boPara == true ){
						break;
					}
				}
			}
		}
	}
	$db->commit();
}



