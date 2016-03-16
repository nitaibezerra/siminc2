<?php
include APPRAIZ."includes/cabecalho.inc";

monta_titulo( $titulo_modulo, '' );
$db->cria_aba( $abacod_tela, $url, '' );

if( $_REQUEST['requisicao'] == 'pesquisar' ) $filtro = ($_REQUEST['dias'] ? '-'.$_REQUEST['dias'] : '');

?>
<form name="formulario" id="formulario" method="post" action="" >
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<table class="tabela" width="100%" cellspacing="0" cellpadding="2" border="0" align="center" style="color:333333;">
		<tr>
			<td class="subtitulodireita">Dia Anterior:</td>
			<td><?
				$dias = $_REQUEST['dias']; 
				echo campo_texto( 'dias', 'N', 'S', '', 10, 10, '[#]', ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%"> </td>
			<td>
				<input type="button" onclick="pesquisarAnalise();" value="Pesquisar"/>
				<input type="button" onclick="window.location.href = window.location" value="Dia Atual"/>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Data de Referência:</td>
			<td><b><? echo $db->pegaUm("select to_char(cast(now() as date) $filtro, 'DD/MM/YYYY')"); ?></b></td>
		</tr>
	</table>
</form>
<script type="text/javascript">
	function pesquisarAnalise(){
		jQuery('[name="requisicao"]').val('pesquisar');
		jQuery('[name="formulario"]').submit();
	}
</script>
<?php

$sql = "select 
			foo.ano||'&nbsp;' as ano,
			count(empid) as totempenho,
		    emp.total as empatualizado,
		    pag.totalpag as totpagamento,
		    pog.total as pagatualizado
		from (
		    select distinct
		        substring(e.empnumeroprocesso, 12, 4) as ano,
		        p.prpid,
		        e.empid
		    from par.processopar p
		        inner join par.empenho e on e.empnumeroprocesso = p.prpnumeroprocesso and e.empstatus = 'A'
		)foo
		left join(
		      select count(pagid) as totalpag, ano from(
		          select distinct
		              pag.pagid,
		              pro.prpid codigo,
		              substring(pro.prpnumeroprocesso, 12, 4) as ano,
		              prpdatapagamentosigef as data
		          from
		              par.processopar pro
		              inner join par.empenho emp on emp.empnumeroprocesso = pro.prpnumeroprocesso
		              inner join par.pagamento pag on pag.empid = emp.empid and pag.pagstatus = 'A' and parnumseqob is not null
		          where
		              pro.prpstatus = 'A'
		              and emp.empstatus = 'A'
		      ) as foo group by ano
		  )pag on pag.ano = foo.ano
		left join(
		      select count(empid) as total, ano from(
		          select distinct
		          	pro.prpid codigo,
		          	emp.empid,
		            substring(pro.prpnumeroprocesso, 12, 4) as ano,
		            prpdataconsultasigef as data
		        from
		            par.processopar pro
		            inner join par.empenho emp on emp.empnumeroprocesso = pro.prpnumeroprocesso
		        where
		            pro.prpstatus = 'A'
		            and emp.empstatus = 'A'
		            and cast(to_char(coalesce(prpdataconsultasigef, '1900-01-01'), 'YYYY-MM-DD') as date) = cast(to_char(cast(now() as date) $filtro , 'YYYY-MM-DD') as date)
		      ) as foo group by ano
		  )emp on emp.ano = foo.ano
		left join(
		      select count(pagid) as total, ano from(
		          select distinct
		              pag.pagid,
		              pro.prpid codigo,
		              substring(pro.prpnumeroprocesso, 12, 4) as ano,
		              prpdatapagamentosigef as data
		          from
		              par.processopar pro
		              inner join par.empenho emp on emp.empnumeroprocesso = pro.prpnumeroprocesso
		              inner join par.pagamento pag on pag.empid = emp.empid and pag.pagstatus = 'A' and parnumseqob is not null
		          where
		              pro.prpstatus = 'A'
		              and emp.empstatus = 'A'
		              and cast(to_char(coalesce(prpdatapagamentosigef, '1900-01-01'), 'YYYY-MM-DD') as date) = cast(to_char(cast(now() as date) $filtro , 'YYYY-MM-DD') as date)
		      ) as foo group by ano
		  )pog on pog.ano = foo.ano
		group by foo.ano, pog.total, emp.total, pag.totalpag
		order by foo.ano";

?>
<table class="listagem" width="100%" cellspacing="0" cellpadding="2" border="0" align="center" style="color:333333;">
	<tr>
		<th>Lista de Empenhos/Pagamentos PAR</th>
	</tr>
</table>
<?php 
$cabecalho = array('Ano', 'Total de Empenhos', 'Total de Empenhos Atualizados', 'Total de Pagamentos', 'Total de Pagamentos Atualizados');
$db->monta_lista_simples( $sql, $cabecalho,100,5,'S','100%','S', true, false, false, true);


$sql = "select 
			foo.ano||'&nbsp;' as ano,
			count(empid) as totempenho,
		    emp.total as empatualizado,
		    pag.totalpag as totpagamento,
		    pog.total as pagatualizado
		from (
		    select distinct
		        substring(e.empnumeroprocesso, 12, 4) as ano,
		        p.proid,
		        e.empid
		    from par.processoobraspar p
		        inner join par.empenho e on e.empnumeroprocesso = p.pronumeroprocesso and e.empstatus = 'A'
		)foo
		left join(
		      select count(pagid) as totalpag, ano from(
		          select distinct
		              pag.pagid,
		              pro.proid codigo,
		              substring(pro.pronumeroprocesso, 12, 4) as ano,
		              prodatapagamentosigef as data
		          from
		              par.processoobraspar pro
		              inner join par.empenho emp on emp.empnumeroprocesso = pro.pronumeroprocesso
		              inner join par.pagamento pag on pag.empid = emp.empid and pag.pagstatus = 'A' and parnumseqob is not null
		          where
		              pro.prostatus = 'A'
		              and emp.empstatus = 'A'
		      ) as foo group by ano
		  )pag on pag.ano = foo.ano
		left join(
		      select count(empid) as total, ano from(
		          select distinct
		          	pro.proid codigo,
		          	emp.empid,
		            substring(pro.pronumeroprocesso, 12, 4) as ano,
		            prodataconsultasigef as data
		        from
		            par.processoobraspar pro
		            inner join par.empenho emp on emp.empnumeroprocesso = pro.pronumeroprocesso
		        where
		            pro.prostatus = 'A'
		            and emp.empstatus = 'A'
		            and cast(to_char(coalesce(prodataconsultasigef, '1900-01-01'), 'YYYY-MM-DD') as date) = cast(to_char(cast(now() as date) $filtro, 'YYYY-MM-DD') as date)
		      ) as foo group by ano
		  )emp on emp.ano = foo.ano
		left join(
		      select count(pagid) as total, ano from(
		          select distinct
		              pag.pagid,
		              pro.proid codigo,
		              substring(pro.pronumeroprocesso, 12, 4) as ano,
		              prodatapagamentosigef as data
		          from
		              par.processoobraspar pro
		              inner join par.empenho emp on emp.empnumeroprocesso = pro.pronumeroprocesso
		              inner join par.pagamento pag on pag.empid = emp.empid and pag.pagstatus = 'A' and parnumseqob is not null
		          where
		              pro.prostatus = 'A'
		              and emp.empstatus = 'A'
		              and cast(to_char(coalesce(prodatapagamentosigef, '1900-01-01'), 'YYYY-MM-DD') as date) = cast(to_char(cast(now() as date) $filtro, 'YYYY-MM-DD') as date)
		      ) as foo group by ano
		  )pog on pog.ano = foo.ano
		group by foo.ano, pog.total, emp.total, pag.totalpag
		order by foo.ano";

?>
<table class="listagem" width="100%" cellspacing="0" cellpadding="2" border="0" align="center" style="color:333333;">
	<tr>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<th>Lista de Empenhos/Pagamentos OBRAS PAR</th>
	</tr>
</table>
<?php 
$cabecalho = array('Ano', 'Total de Empenhos', 'Total de Empenhos Atualizados', 'Total de Pagamentos', 'Total de Pagamentos Atualizados');
$db->monta_lista_simples( $sql, $cabecalho,100,5,'S','100%','S', true, false, false, true);

$sql = "select 
			foo.ano||'&nbsp;' as ano,
			count(empid) as totempenho,
		    emp.total as empatualizado,
		    pag.totalpag as totpagamento,
		    pog.total as pagatualizado
		from (
		    select distinct
		        substring(e.empnumeroprocesso, 12, 4) as ano,
		        p.proid,
		        e.empid
		    from par.processoobra p
		        inner join par.empenho e on e.empnumeroprocesso = p.pronumeroprocesso and e.empstatus = 'A'
		)foo
		left join(
		      select count(pagid) as totalpag, ano from(
		          select distinct
		              pag.pagid,
		              pro.proid codigo,
		              substring(pro.pronumeroprocesso, 12, 4) as ano,
		              prodatapagamentosigef as data
		          from
		              par.processoobra pro
		              inner join par.empenho emp on emp.empnumeroprocesso = pro.pronumeroprocesso
		              inner join par.pagamento pag on pag.empid = emp.empid and pag.pagstatus = 'A' and parnumseqob is not null
		          where
		              pro.prostatus = 'A'
		              and emp.empstatus = 'A'
		      ) as foo group by ano
		  )pag on pag.ano = foo.ano
		left join(
		      select count(empid) as total, ano from(
		          select distinct
		          	pro.proid codigo,
		          	emp.empid,
		            substring(pro.pronumeroprocesso, 12, 4) as ano,
		            prodataconsultasigef as data
		        from
		            par.processoobra pro
		            inner join par.empenho emp on emp.empnumeroprocesso = pro.pronumeroprocesso
		        where
		            pro.prostatus = 'A'
		            and emp.empstatus = 'A'
		            and cast(to_char(coalesce(prodataconsultasigef, '1900-01-01'), 'YYYY-MM-DD') as date) = cast(to_char(cast(now() as date) $filtro, 'YYYY-MM-DD') as date)
		      ) as foo group by ano
		  )emp on emp.ano = foo.ano
		left join(
		      select count(pagid) as total, ano from(
		          select distinct
		              pag.pagid,
		              pro.proid codigo,
		              substring(pro.pronumeroprocesso, 12, 4) as ano,
		              prodatapagamentosigef as data
		          from
		              par.processoobra pro
		              inner join par.empenho emp on emp.empnumeroprocesso = pro.pronumeroprocesso
		              inner join par.pagamento pag on pag.empid = emp.empid and pag.pagstatus = 'A' and parnumseqob is not null
		          where
		              pro.prostatus = 'A'
		              and emp.empstatus = 'A'
		              and cast(to_char(coalesce(prodatapagamentosigef, '1900-01-01'), 'YYYY-MM-DD') as date) = cast(to_char(cast(now() as date) $filtro, 'YYYY-MM-DD') as date)
		      ) as foo group by ano
		  )pog on pog.ano = foo.ano
		group by foo.ano, pog.total, emp.total, pag.totalpag
		order by foo.ano";

?>
<table class="listagem" width="100%" cellspacing="0" cellpadding="2" border="0" align="center" style="color:333333;">
	<tr>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<th>Lista de Empenhos/Pagamentos OBRAS PAC</th>
	</tr>
</table>
<?php 
$cabecalho = array('Ano', 'Total de Empenhos', 'Total de Empenhos Atualizados', 'Total de Pagamentos', 'Total de Pagamentos Atualizados');
$db->monta_lista_simples( $sql, $cabecalho,100,5,'S','100%','S', true, false, false, true);
?>