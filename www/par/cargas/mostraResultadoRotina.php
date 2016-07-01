<?php

ini_set("memory_limit", "3024M");
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";

include_once APPRAIZ . "includes/classes_simec.inc";

/**** DECLARAÇÃO DE VARIAVEIS ****/
session_start();
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] 	= '';
$_SESSION['usucpf'] 		= '';

$db 				= new cls_banco();

$filtro = '';
if( !empty($_REQUEST['dias']) ) $filtro = $_REQUEST['dias'];

$sql = "select distinct
			count(empid) as totalemp,
		    sum(totpag) as totalpag,
		    tipo
		from(
		    select
		        p.prpid as codigo,
		        p.prpnumeroprocesso as processo,
		        e.empid,
		        count(pg.pagid) as totpag,
		        'PAR' as tipo
		    from par.processopar p 
		    	inner join par.empenho e on e.empnumeroprocesso = p.prpnumeroprocesso and e.empstatus = 'A'
		        left join par.pagamento pg on pg.empid = e.empid and pg.pagstatus = 'A'
		    where p.prpstatus = 'A'
		    group by p.prpid, p.prpnumeroprocesso, e.empid
		    union all
		    select
		        p.proid as codigo,
		        p.pronumeroprocesso as processo,
		        e.empid,
		        count(pg.pagid) as totpag,
		        'OBRAS' as tipo
		    from par.processoobraspar p 
		    	inner join par.empenho e on e.empnumeroprocesso = p.pronumeroprocesso and e.empstatus = 'A'
		        left join par.pagamento pg on pg.empid = e.empid and pg.pagstatus = 'A'
		    where p.prostatus = 'A'
		    group by p.proid, p.pronumeroprocesso, e.empid
		    union all
		    select
		        p.proid as codigo,
		        p.pronumeroprocesso as processo,
		        e.empid,
		        count(pg.pagid) as totpag,
		        'PAC' as tipo
		    from par.processoobra p 
		    	inner join par.empenho e on e.empnumeroprocesso = p.pronumeroprocesso and e.empstatus = 'A'
			    left join par.pagamento pg on pg.empid = e.empid and pg.pagstatus = 'A'
		    where p.prostatus = 'A'
		    group by p.proid, p.pronumeroprocesso, e.empid
		) as foo
		group by tipo
		order by tipo desc";
$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

$arrAtualiza = carregarDadosAtualizado();

$arrRegistro = array();
foreach ($arrDados as $v) {
	if( $v['tipo'] == 'PAR' ) $arrRegistro['PAR'] = array('qtdtotalemp' => $v['totalemp'], 
														'atualizado' => $arrAtualiza['qtdAtualPar'], 
														'qtdNaoAtualizado' => ((int)$v['totalemp'] - (int)$arrAtualiza['qtdAtualPar']),
														'qtdtotalpag' => $v['totalpag'],
														'atualizadoPag' => $arrAtualiza['qtdAtualPagPar'], 
														'qtdNaoAtualizadoPag' => ((int)$v['totalpag'] - (int)$arrAtualiza['qtdAtualPagPar']),
													);
	if( $v['tipo'] == 'OBRAS' ) $arrRegistro['OBRAS'] = array('qtdtotalemp' => $v['totalemp'], 
														'atualizado' => $arrAtualiza['qtdAtualObra'], 
														'qtdNaoAtualizado' => ((int)$v['totalemp'] - (int)$arrAtualiza['qtdAtualObra']), 
														'qtdtotalpag' => $v['totalpag'],
														'atualizadoPag' => $arrAtualiza['qtdAtualPagObra'], 
														'qtdNaoAtualizadoPag' => ((int)$v['totalpag'] - (int)$arrAtualiza['qtdAtualPagObra']),
													);
	if( $v['tipo'] == 'PAC' ) $arrRegistro['PAC'] = array('qtdtotalemp' => $v['totalemp'], 
														'atualizado' => $arrAtualiza['qtdAtualPac'], 
														'qtdNaoAtualizado' => ((int)$v['totalemp'] - (int)$arrAtualiza['qtdAtualPac']), 
														'qtdtotalpag' => $v['totalpag'],
														'atualizadoPag' => $arrAtualiza['qtdAtualPagPac'], 
														'qtdNaoAtualizadoPag' => ((int)$v['totalpag'] - (int)$arrAtualiza['qtdAtualPagPac']),
													);
}
?>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
<table width="100%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
<tr>
	<td>
		<table width="50%" align="center" border="0" class="listagem" cellpadding="3" cellspacing="1">
			<tr>
				<td class="SubTitulocentro" colspan="4">Registro de Atualização Empenho</td>
			</tr>
			<tr>
				<th>Sistema</th>
				<th>Total Atual</th>
				<th>Total Atualizado</th>
				<th>Total não Atualizado</th>
			</tr>
	<?php
		$qtdAtual = 0; 
		$qtdAtualizado = 0; 
		$qtdAtualN = 0; 
		foreach ($arrRegistro as $sistema => $v) {
			$qtdAtual = (int)$qtdAtual + (int)$v['qtdtotalemp'];
			$qtdAtualizado = (int)$qtdAtualizado + (int)$v['atualizado'];
			$qtdAtualN = (int)$qtdAtualN + (int)$v['qtdNaoAtualizado'];
		?>
			<tr>
				<td><?php echo $sistema; ?></td>
				<td style="color: rgb(0, 102, 204);"><?php echo $v['qtdtotalemp']; ?></td>
				<td style="color: rgb(0, 102, 204);"><?php echo $v['atualizado']; ?></td>
				<td style="color: rgb(0, 102, 204);"><?php echo $v['qtdNaoAtualizado']; ?></td>
			</tr>	
	<?php }?>
			<tr>
				<td align="right"><b>Total Geral:</b></td>
				<td style="color: rgb(0, 102, 204);"><?php echo $qtdAtual;?></td>
				<td style="color: rgb(0, 102, 204);"><?php echo $qtdAtualizado;?></td>
				<td style="color: rgb(0, 102, 204);"><?php echo $qtdAtualN;?></td>
			</tr>
		</table>
	</td>
	<td>
		<table width="50%" align="center" border="0" class="listagem" cellpadding="3" cellspacing="1">
			<tr>
				<td class="SubTitulocentro" colspan="4">Registro de Atualização Pagamento</td>
			</tr>
			<tr>
				<th>Sistema</th>
				<th>Total Atual</th>
				<th>Total Atualizado</th>
				<th>Total não Atualizado</th>
			</tr>
	<?php
		reset($arrRegistro); 
		$qtdAtual = 0; 
		$qtdAtualizado = 0; 
		$qtdAtualN = 0; 
		foreach ($arrRegistro as $sistema => $v) {
			$qtdAtual = (int)$qtdAtual + (int)$v['qtdtotalpag'];
			$qtdAtualizado = (int)$qtdAtualizado + (int)$v['atualizadoPag'];
			$qtdAtualN = (int)$qtdAtualN + (int)$v['qtdNaoAtualizadoPag']; ?>
			<tr>
				<td><?php echo $sistema; ?></td>
				<td style="color: rgb(0, 102, 204);"><?php echo $v['qtdtotalpag']; ?></td>
				<td style="color: rgb(0, 102, 204);"><?php echo $v['atualizadoPag']; ?></td>
				<td style="color: rgb(0, 102, 204);"><?php echo $v['qtdNaoAtualizadoPag']; ?></td>
			</tr>	
	<?php }?>
			<tr>
				<td align="right"><b>Total Geral:</b></td>
				<td style="color: rgb(0, 102, 204);"><?php echo $qtdAtual;?></td>
				<td style="color: rgb(0, 102, 204);"><?php echo $qtdAtualizado;?></td>
				<td style="color: rgb(0, 102, 204);"><?php echo $qtdAtualN;?></td>
			</tr>
		</table>
	</td>
</tr>
</table>
<?php 

$arrProcesso = cargaProcessoEmpenho( $filtro );

?>
<table width="100%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
<tr>
	<td>
		<table width="50%" align="center" border="0" class="listagem" cellpadding="3" cellspacing="1">
			<tr>
				<td class="SubTitulocentro" colspan="10">Registro de Carga SIMEC/SIGEF de Empenho</td>
			</tr>
			<tr>
				<th>Sistema</th>
				<th>Ano</th>
				<th>Total Atual</th>
				<th>Total Atualizado</th>
				<th>Total não Atualizado</th>
			</tr>
<?php	
		$qtdAtual = 0;
		$qtdAtualizado = 0;
		$qtdAtualN = 0;
		foreach ($arrProcesso as $sistema => $arSistema) {
			foreach ($arSistema as $key => $v) {
				$qtdAtual = (int)$qtdAtual + (int)$v['total'];
				$qtdAtualizado = (int)$qtdAtualizado + (int)$v['qtdatualizado'];
				$qtdAtualN = (int)$qtdAtualN + ((int)$v['total'] - (int)$v['qtdatualizado']);
				
				$sistema == 'obra' ? $cor = "#dedfde" : $cor = "";
				
				echo '<tr bgcolor='.$cor.' >';
				if( $key == 0 ){ ?>
					<td rowspan="4" valign="middle" align="center"><?php echo str_to_upper($sistema); ?></td>
<?php 			}?>				
				<td style="color: rgb(0, 102, 204);"><?php echo $v['ano'];?></td>
				<td style="color: rgb(0, 102, 204);"><?php echo $v['total'];?></td>
				<td style="color: rgb(0, 102, 204);"><?php echo $v['qtdatualizado'];?></td>
				<td style="color: rgb(0, 102, 204);"><?php echo ((int)$v['total'] - (int)$v['qtdatualizado']);?></td>
			</tr>
<?php 		}		
		} ?>
			<tr>
				<td align="right" colspan="2"><b>Total Geral:</b></td>
				<td style="color: rgb(0, 102, 204);"><b><?php echo $qtdAtual;?></b></td>
				<td style="color: rgb(0, 102, 204);"><b><?php echo $qtdAtualizado;?></b></td>
				<td style="color: rgb(0, 102, 204);"><b><?php echo $qtdAtualN;?></b></td>
			</tr>
		</table>
	</td>
	<td>
		<table width="50%" align="center" border="0" class="listagem" cellpadding="3" cellspacing="1">
			<tr>
				<td class="SubTitulocentro" colspan="10">Registro de Carga SIMEC/SIGEF de Pagamento</td>
			</tr>
			<tr>
				<th>Sistema</th>
				<th>Ano</th>
				<th>Total Atual</th>
				<th>Total Atualizado</th>
				<th>Total não Atualizado</th>
			</tr>
<?php	

$arrProcessoPag = cargaProcessoPagamento( $filtro );

		$qtdAtual = 0;
		$qtdAtualizado = 0;
		$qtdAtualN = 0;
		foreach ($arrProcessoPag as $sistema => $arSistema) {
			foreach ($arSistema as $key => $v) {
				$qtdAtual = (int)$qtdAtual + (int)$v['total'];
				$qtdAtualizado = (int)$qtdAtualizado + (int)$v['qtdatualizado'];
				$qtdAtualN = (int)$qtdAtualN + ((int)$v['total'] - (int)$v['qtdatualizado']);
				
				$sistema == 'obra' ? $cor = "#dedfde" : $cor = "";
				
				echo '<tr bgcolor='.$cor.' >';
				if( $key == 0 ){ ?>
					<td rowspan="4" valign="middle" align="center"><?php echo str_to_upper($sistema); ?></td>
<?php 			}?>				
				<td style="color: rgb(0, 102, 204);"><?php echo $v['ano'];?></td>
				<td style="color: rgb(0, 102, 204);"><?php echo $v['total'];?></td>
				<td style="color: rgb(0, 102, 204);"><?php echo $v['qtdatualizado'];?></td>
				<td style="color: rgb(0, 102, 204);"><?php echo ((int)$v['total'] - (int)$v['qtdatualizado']);?></td>
			</tr>
<?php 		}		
		} ?>
			<tr>
				<td align="right" colspan="2"><b>Total Geral:</b></td>
				<td style="color: rgb(0, 102, 204);"><b><?php echo $qtdAtual;?></b></td>
				<td style="color: rgb(0, 102, 204);"><b><?php echo $qtdAtualizado;?></b></td>
				<td style="color: rgb(0, 102, 204);"><b><?php echo $qtdAtualN;?></b></td>
			</tr>
		</table>
	</td>
</tr>
</table>
<?

function carregarDadosAtualizado(){
	global $db;
	
	$sql = "select
			    count(empid) as total,
	            (select distinct
                      count(pg.pagid)
                  from
                      par.empenho emp1
                      inner join par.pagamento pg on pg.empid = emp1.empid and pg.pagstatus = 'A'
                      inner join par.processopar po on po.prpnumeroprocesso = emp1.empnumeroprocesso and po.prpstatus = 'A'
                  where
                      emp1.empstatus = 'A'
                      and cast(to_char(pg.pagdataatualizacao, 'YYYY-MM-DD') as date) = cast(to_char(now(), 'YYYY-MM-DD') as date)) as totalpag
			from(
			    select distinct
			        emp.empid,
			        to_char(coalesce(emp.empdataatualizacao, '1900-01-01'), 'DD/MM/YYYY') as data
			    from
			        par.empenho emp
			        inner join par.processopar pro on pro.prpnumeroprocesso = emp.empnumeroprocesso
			    where
			        emp.empstatus = 'A'
			        and pro.prpstatus = 'A'
	            group by emp.empdataatualizacao, emp.empid
			) as foo
			where
				cast(data as date) = cast(to_char(now(), 'YYYY-MM-DD') as date)";
	$qtdAtualPar = $db->pegaLinha($sql);
		
	$sql = "select
			    count(empid) as total,
	            (select distinct
                      count(pg.pagid)
                  from
                      par.empenho emp1
                      inner join par.pagamento pg on pg.empid = emp1.empid and pg.pagstatus = 'A'
                      inner join par.processoobraspar po on po.pronumeroprocesso = emp1.empnumeroprocesso and po.prostatus = 'A'
                  where
                      emp1.empstatus = 'A'
                      and cast(to_char(pg.pagdataatualizacao, 'YYYY-MM-DD') as date) = cast(to_char(now(), 'YYYY-MM-DD') as date)) as totalpag
			from(
			    select distinct
			        emp.empid,
			        to_char(coalesce(emp.empdataatualizacao, '1900-01-01'), 'DD/MM/YYYY') as data
			    from
			        par.empenho emp
			        inner join par.processoobraspar pro on pro.pronumeroprocesso = emp.empnumeroprocesso
			    where
			        emp.empstatus = 'A'
			        and pro.prostatus = 'A'
	            group by emp.empdataatualizacao, emp.empid
			) as foo
			where
				cast(data as date) = cast(to_char(now(), 'YYYY-MM-DD') as date)";
	$qtdAtualObra = $db->pegaLinha($sql);
		
	$sql = "select
			    count(empid) as total,
	            (select distinct
                      count(pg.pagid)
                  from
                      par.empenho emp1
                      inner join par.pagamento pg on pg.empid = emp1.empid and pg.pagstatus = 'A'
                      inner join par.processoobra po on po.pronumeroprocesso = emp1.empnumeroprocesso and po.prostatus = 'A'
                  where
                      emp1.empstatus = 'A'
                      and cast(to_char(pg.pagdataatualizacao, 'YYYY-MM-DD') as date) = cast(to_char(now(), 'YYYY-MM-DD') as date)) as totalpag
			from(
			    select distinct
			        emp.empid,
			        to_char(coalesce(emp.empdataatualizacao, '1900-01-01'), 'DD/MM/YYYY') as data
			    from
			        par.empenho emp
			        inner join par.processoobra pro on pro.pronumeroprocesso = emp.empnumeroprocesso
			    where
			        emp.empstatus = 'A'
			        and pro.prostatus = 'A'
			    group by emp.empdataatualizacao, emp.empid
			) as foo
			where
				cast(data as date) = cast(to_char(now(), 'YYYY-MM-DD') as date)";
	$qtdAtualPac = $db->pegaLinha($sql);
	
	return array('qtdAtualPar' => $qtdAtualPar['total'],  
				'qtdAtualPagPar' => $qtdAtualPar['totalpag'],
				'qtdAtualObra' => $qtdAtualObra['total'],  
				'qtdAtualPagObra' => $qtdAtualObra['totalpag'],  
				'qtdAtualPac' => $qtdAtualPac['total'],
				'qtdAtualPagPac' => $qtdAtualPac['totalpag'],
			);
}

function cargaProcessoEmpenho( $filtro = '' ){
	global $db;
	
	$arrRetorno = array();
	
	if( !empty($filtro) ){
		$filtro = " - $filtro";
	} else {
		$filtro = "";
	}
	
	$sql = "select
			    count(empid) as total,
			    ano,
			    (select count(distinct e1.empid) from
			        par.processopar p1
			        inner join par.empenhosigef e1 on e1.ems_numero_processo = p1.prpnumeroprocesso and p1.prpstatus = 'A'
			     where
			        substring(e1.ems_numero_processo, 12, 4) = foo.ano
			        and cast(to_char(coalesce(e1.ems_data_atualizacao_rotina, '1900-01-01'), 'YYYY-MM-DD') as date) = (cast(to_char(now(), 'YYYY-MM-DD') as date) $filtro)
			     ) as qtdatualizado
			from(
			    select distinct
			        emp.empid,
			        substring(emp.empnumeroprocesso, 12, 4) as ano
			    from
			        par.processopar pro
			        inner join par.empenho emp on emp.empnumeroprocesso = pro.prpnumeroprocesso
			    where
			        pro.prpstatus = 'A'
			        and emp.empstatus = 'A'
			) as foo
			group by ano
			order by ano";
	$arPar = $db->carregar($sql);
	$arrRetorno['par'] = $arPar;
	 
	$sql = "select
			    count(empid) as total,
			    ano,
			    (select count(distinct e1.empid) from
                    par.processoobraspar p1
                    inner join par.empenhosigef e1 on e1.ems_numero_processo = p1.pronumeroprocesso
                 where
                    substring(e1.ems_numero_processo, 12, 4) = foo.ano
                    and cast(to_char(coalesce(e1.ems_data_atualizacao_rotina, '1900-01-01'), 'YYYY-MM-DD') as date) = (cast(to_char(now(), 'YYYY-MM-DD') as date) $filtro)
                 ) as qtdatualizado
			from(
			    select distinct
			        emp.empid,
			        substring(emp.empnumeroprocesso, 12, 4) as ano
			    from
			        par.processoobraspar pro
			        inner join par.empenho emp on emp.empnumeroprocesso = pro.pronumeroprocesso
			    where
			        pro.prostatus = 'A'
			        and emp.empstatus = 'A'
			) as foo
			group by ano
			order by ano";
	$arObra = $db->carregar($sql);
	$arrRetorno['obra'] = $arObra;
	
	$sql = "select
			    count(empid) as total,
			    ano,
			    (select count(distinct e1.empid) from
                    par.processoobra p1
                    inner join par.empenhosigef e1 on e1.ems_numero_processo = p1.pronumeroprocesso
                 where
                    substring(e1.ems_numero_processo, 12, 4) = foo.ano
                    and cast(to_char(coalesce(e1.ems_data_atualizacao_rotina, '1900-01-01'), 'YYYY-MM-DD') as date) = (cast(to_char(now(), 'YYYY-MM-DD') as date) $filtro)
                 ) as qtdatualizado
			from(
			    select distinct
			        emp.empid,
			        substring(emp.empnumeroprocesso, 12, 4) as ano
			    from
			        par.processoobra pro
			        inner join par.empenho emp on emp.empnumeroprocesso = pro.pronumeroprocesso
			    where
			        pro.prostatus = 'A'
			        and emp.empstatus = 'A'
			) as foo
			group by ano 
			order by ano";	
	$arPac = $db->carregar($sql);
	$arrRetorno['pac'] = $arPac;
	
	return $arrRetorno;
}

function cargaProcessoPagamento( $filtro = '' ){
	global $db;
	
	$arrRetorno = array();
	
	if( !empty($filtro) ){
		$filtro = " - $filtro";
	} else {
		$filtro = "";
	}
	
	$sql = "select
			    count(pagid) as total,
			    ano,
			    (select count(distinct e1.empid) from
			        par.processopar p1
			        inner join par.historicopagamentosigef e1 on e1.nu_processo = p1.prpnumeroprocesso and p1.prpstatus = 'A'
			     where
			        substring(e1.nu_processo, 12, 4) = foo.ano
			        and cast(to_char(coalesce(e1.data_atualizacao_rotina, '1900-01-01'), 'YYYY-MM-DD') as date) = (cast(to_char(now(), 'YYYY-MM-DD') as date) $filtro)
			     ) as qtdatualizado
			from(
			    select distinct
			        pag.pagid,
			        substring(emp.empnumeroprocesso, 12, 4) as ano
			    from
			        par.processopar pro
			        inner join par.empenho emp on emp.empnumeroprocesso = pro.prpnumeroprocesso
                    inner join par.pagamento pag on pag.empid = emp.empid and pag.pagstatus = 'A'
			    where
			        pro.prpstatus = 'A'
			        and emp.empstatus = 'A'
			) as foo
			group by ano
			order by ano";
	$arPar = $db->carregar($sql);
	$arrRetorno['par'] = $arPar;
	 
	$sql = "select
			    count(pagid) as total,
			    ano,
			    (select count(distinct e1.empid) from
			        par.processoobraspar p1
			        inner join par.historicopagamentosigef e1 on e1.nu_processo = p1.pronumeroprocesso and p1.prostatus = 'A'
			     where
			        substring(e1.nu_processo, 12, 4) = foo.ano
			        and cast(to_char(coalesce(e1.data_atualizacao_rotina, '1900-01-01'), 'YYYY-MM-DD') as date) = (cast(to_char(now(), 'YYYY-MM-DD') as date) $filtro)
			     ) as qtdatualizado
			from(
			    select distinct
			        pag.pagid,
			        substring(emp.empnumeroprocesso, 12, 4) as ano
			    from
			        par.processoobraspar pro
			        inner join par.empenho emp on emp.empnumeroprocesso = pro.pronumeroprocesso
			        inner join par.pagamento pag on pag.empid = emp.empid and pag.pagstatus = 'A'
			    where
			        pro.prostatus = 'A'
			        and emp.empstatus = 'A'
			) as foo
			group by ano
			order by ano";
	$arObra = $db->carregar($sql);
	$arrRetorno['obra'] = $arObra;
	
	$sql = "select
			    count(pagid) as total,
			    ano,
			    (select count(distinct e1.empid) from
			        par.processoobraspar p1
			        inner join par.historicopagamentosigef e1 on e1.nu_processo = p1.pronumeroprocesso and p1.prostatus = 'A'
			     where
			        substring(e1.nu_processo, 12, 4) = foo.ano
			        and cast(to_char(coalesce(e1.data_atualizacao_rotina, '1900-01-01'), 'YYYY-MM-DD') as date) = (cast(to_char(now(), 'YYYY-MM-DD') as date) $filtro)
			     ) as qtdatualizado
			from(
			    select distinct
			        pag.pagid,
			        substring(emp.empnumeroprocesso, 12, 4) as ano
			    from
			        par.processoobra pro
			        inner join par.empenho emp on emp.empnumeroprocesso = pro.pronumeroprocesso
                    inner join par.pagamento pag on pag.empid = emp.empid and pag.pagstatus = 'A'
			    where
			        pro.prostatus = 'A'
			        and emp.empstatus = 'A'
			) as foo
			group by ano 
			order by ano";	
	$arPac = $db->carregar($sql);
	$arrRetorno['pac'] = $arPac;
	
	return $arrRetorno;
}
?>