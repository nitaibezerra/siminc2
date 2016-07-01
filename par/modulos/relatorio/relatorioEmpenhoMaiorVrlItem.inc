<?php

include APPRAIZ . 'includes/cabecalho.inc';

monta_titulo('Empenho Maior que Valor de Obras/Subação', '');
?>
<form method="post" name="formulario" id="formulario">
	<input type="hidden" id="exporta" name="exporta" value="<?=$exporta; ?>">
    <table id="pesquisas" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tbody>
        <tr>
            <td class="SubTituloDireita">Tipo Processo:</td>
            <td>
                <input type="radio" name="sistema" value="PAR" <?php echo $_POST['sistema'] == 'PAR' ? 'checked="checked"' : ''; ?> />Subações Genérico do PAR
                <input type="radio" name="sistema" value="OBRA" style="margin-left: 10px;" <?php echo $_POST['sistema'] == 'OBRA' ? 'checked="checked"' : ''; ?> />Obras do PAR
                <input type="radio" name="sistema" value="PAC" style="margin-left: 10px;" <?php echo $_POST['sistema'] == 'PAC' ? 'checked="checked"' : ''; ?> />Obras do PAC
            </td>
        </tr>
        <!--<tr>
            <td class="SubTituloDireita" >Estado:</td>
            <td colspan="3">
                <?php
                $estuf = $_POST['estuf'];
                $sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
                $db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" >Município:</td>
            <td colspan="3">
                <?php
                $filtro = simec_htmlentities( $_POST['municipio'] );
                $municipio = $filtro;
                echo campo_texto( 'mundescricao', 'N', 'S', '', 50, 200, '', '');
                ?>
            </td>
        </tr>-->
        <tr>
            <td colspan="2" align="center">
                <input type="button" name="enviar" id="enviar" value="Gerar Relatório"/>
                <input type="button" name="excel" id="excel" value="Gerar Excel">
            </td>
        </tr>
    </table>
</form>
<script type="text/javascript">
    jQuery(function(){
    	jQuery('#excel').click(function(){
    		document.getElementById('exporta').value = "true";
    		jQuery('#formulario').submit();
        });
    	jQuery('#enviar').click(function(){
    		document.getElementById('exporta').value = "false";
    		jQuery('#formulario').submit();
        });
    });        
</script>
<?php 

if( $_POST['sistema'] ){
	if( $_POST['sistema'] == 'PAR'){
		$sql = "select
					processo,
				    sbaid,
				    descricao,
				    eobano||'&nbsp;' as ano,
					vrlsubacao,
				    vrlempenho
				from(
				select
				    e.empnumeroprocesso as processo,
				    es.sbaid,
				    s.sbadsc as descricao,
				    es.eobano,
				    ((sum(es.eobvalorempenho) + sum(coalesce(ref.reevlr,0))) - sum(coalesce(can.vrlcancelado, 0))) as vrlempenho,
				    par.recuperavalorvalidadossubacaoporano(es.sbaid, es.eobano) as vrlsubacao
				from
					par.empenho e
				    inner join par.empenhosubacao es on es.empid = e.empid and es.eobstatus = 'A'
				    inner join par.subacao s on s.sbaid = es.sbaid and s.sbastatus = 'A'
				    left join (
				            select ep.empnumeroprocesso, ep.empidpai, sum(ep.empvalorempenho) as reevlr, ep.empcodigoespecie, es1.sbaid, es1.eobano
				            from par.empenho ep
				                inner join par.empenhosubacao es1 on es1.empid = ep.empid
				            where ep.empcodigoespecie in ('02')
				            group by 
				                ep.empnumeroprocesso,
				                ep.empcodigoespecie,
				                ep.empidpai, es1.sbaid, es1.eobano
				    ) as ref on ref.empidpai = e.empid and ref.sbaid = es.sbaid and ref.eobano = es.eobano
				    left join (
				        select ep.empnumeroprocesso, ep.empidpai, sum(ep.empvalorempenho) as vrlcancelado, ep.empcodigoespecie, es1.sbaid, es1.eobano
				        from par.empenho ep
				            inner join par.empenhosubacao es1 on es1.empid = ep.empid
				        where ep.empcodigoespecie in ('03', '04', '13')
				        group by 
				            ep.empnumeroprocesso,
				            ep.empcodigoespecie,
				            ep.empidpai, es1.sbaid, es1.eobano
				    ) as can on can.empidpai = e.empid and can.sbaid = es.sbaid and can.eobano = es.eobano
				where
					e.empstatus = 'A'
				    and e.empcodigoespecie not in ('02', '03', '04', '13')
				group by
					e.empnumeroprocesso,
				    es.sbaid,
				    es.eobano, s.sbadsc
				) as foo
				where
					vrlempenho > vrlsubacao";
		
		$cabecalho = array('Processo', 'Código', 'Descrição', 'Ano', 'Valor da Subação', 'Valor Empenhado da Subação');
	}
	if( $_POST['sistema'] == 'OBRA'){
		$sql = "SELECT
					po.pronumeroprocesso as processo,
				    v.preid,
				    p.predescricao,
				    cast(p.prevalorobra as numeric(20,2)) as prevalorobra,
				    sum(v.saldo) as saldo
				FROM
					par.processoobraspar po
			    INNER JOIN par.processoobrasparcomposicao 	pc ON pc.proid = po.proid AND pc.pocstatus = 'A'
			    INNER JOIN par.vm_saldo_empenho_por_obra 	v ON v.preid = pc.preid
			    INNER JOIN obras.preobra 					p ON p.preid = v.preid AND p.prestatus = 'A'
				WHERE
    				po.prostatus = 'A'
				GROUP BY v.preid, p.prevalorobra, p.predescricao, po.pronumeroprocesso
				HAVING SUM(v.saldo) > CAST(p.prevalorobra as numeric(20,2))";
		
		$cabecalho = array('Processo', 'Código', 'Descrição', 'Valor da Obra', 'Valor Empenhado da Obra');
	}
	
	if( $_POST['sistema'] == 'PAC'){
		$sql = "SELECT
					po.pronumeroprocesso as processo,
				    v.preid,
				    p.predescricao,
				    cast(p.prevalorobra as numeric(20,2)) as prevalorobra,
				    sum(v.saldo) as saldo
				FROM
					par.processoobra po
 			    INNER JOIN par.processoobraspaccomposicao 	pc ON pc.proid = po.proid AND pc.pocstatus = 'A'
			    INNER JOIN par.vm_saldo_empenho_por_obra 	v ON v.preid = pc.preid
			    INNER JOIN obras.preobra 					p ON p.preid = v.preid AND p.prestatus = 'A'
				WHERE
    				po.prostatus = 'A'
				GROUP BY v.preid, p.prevalorobra, p.predescricao, po.pronumeroprocesso
				HAVING SUM(v.saldo) > CAST(p.prevalorobra as numeric(20,2))";
		
		$cabecalho = array('Processo', 'Código', 'Descrição', 'Valor da Obra', 'Valor Empenhado da Obra');
	}
	
	if( $_POST['exporta'] == 'true' ){
		ob_clean();
		header('content-type: text/html; charset=ISO-8859-1');
		$formato = array('', '', '', '', '', 'n', '', '');
		$db->sql_to_excel($sql, 'relEmpenhoSubacaoObra', $cabecalho, $formato);
	} else {
		$db->monta_lista_simples($sql,$cabecalho, 5000000, 5, 'N', '100%', 'S', true, false, false, true);
	}
}
?>