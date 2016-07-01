<?php
//
// $Id$
//

include_once APPRAIZ . 'includes/workflow.php';

if ( $_REQUEST['estuf'] && $_REQUEST['evento'] == 'selecionar' ) {
    if( cte_selecionarUf( INSTRUMENTO_DIAGNOSTICO_ESTADUAL, WF_TIPO_BRASILPRO, $_REQUEST['estuf'] ) ) {
        $db->commit();
        header( "Location: ?modulo=principal/estrutura_avaliacao&acao=A" );
        exit();
    }
} elseif ( $_REQUEST['muncod'] && $_REQUEST['evento'] == 'selecionar' ) {
    if( cte_selecionarMunicipio( INSTRUMENTO_DIAGNOSTICO_MUNICIPAL, WF_TIPO_BRASILPRO, $_REQUEST['muncod'] ) ) {
        $db->commit();
        header( "Location: ?modulo=principal/estrutura_avaliacao&acao=A" );
        exit();
    }
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
echo '<br />';
$db->cria_aba($abacod_tela, $url, '');
cte_montaTitulo($titulo_modulo, '&nbsp;');

if ( $_REQUEST["acao"] == "M" ) {

?>

    <script type="text/javascript">
        function removerFiltro(){
            document.formulario.filtro.value        = "";
            document.formulario.estuf.selectedIndex = 0;
            document.formulario.submit();
        }
    </script>

    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tbody>
            <tr>
                <td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
                    <form action="" method="get" name="formulario">
                        <input type="hidden" name="modulo" value="<?= $_REQUEST['modulo'] ?>"/>
                        <input type="hidden" name="acao" value="<?= $_REQUEST['acao'] ?>"/>
                        <div style="float: left;">
                            
                            <table border="0" cellpadding="3" cellspacing="0">
                                <tr>
                                    <td valign="bottom">
                                        Município
                                        <br/>
                                        <?php $filtro = simec_htmlentities( $_REQUEST['filtro'] ); ?>
                                        <?= campo_texto( 'filtro', 'N', 'S', '', 50, 200, '', '' ); ?>
                                    </td>
                                    <td valign="bottom">
                                        Estado
                                        <br/>
                                        <?php
                                        $estuf = $_REQUEST['estuf'];
                                        $unidades = "'" . implode( "','", cte_pegarUfsPermitidas() ) . "'";
                                        $sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e where e.estuf in ({$unidades}) order by e.estdescricao asc";
                                        $db->monta_combo( 'estuf', $sql, 'S', 'Todos as Unidades Federais', '', '' );
                                        ?>
                                    </td>
                                    <td valign="bottom">
                                        <?php
                                        $par = $_REQUEST['par'] ? $_REQUEST['par'] : "com";
                                        ?>
                                        <label><input type="radio" name="par" value="com"<?php echo $par == "com" ? ' checked="checked"' : '' ?> />Com PAR</label>
                                        <label><input type="radio" name="par" value="sem"<?php echo $par == "sem" ? ' checked="checked"' : '' ?> />Sem PAR</label>
                                        &nbsp;&nbsp;|&nbsp;&nbsp;
                                        <label><input type="checkbox" name="capitais" value="1" class="normal" style="margin-top: -1px;" <?= $_REQUEST['capitais'] == 1 ? 'checked="checked"' : '' ?>/>Capitais</label>
                                        <label><input type="checkbox" name="grandescidades" value="1" class="normal" style="margin-top: -1px;" <?= $_REQUEST['grandescidades'] == 1 ? 'checked="checked"' : '' ?>/>Grandes Cidades</label>
                                    </td>
                                </tr>
                                <tr>
                                    <td colspan="3">
                                        <label style="font-weight: bold;"><input type="radio" name="analise" value="" <?= $_REQUEST["analise"] == "" ? 'checked="checked"' : '' ?>/>Todas</label>
                                        <label><input type="radio" name="analise" value="naoanalisado" <?= $_REQUEST["analise"] == "naoanalisado" ? 'checked="checked"' : '' ?>/>Não Analisado</label>
                                        <label><input type="radio" name="analise" value="emanalise" <?= $_REQUEST["analise"] == "emanalise" ? 'checked="checked"' : '' ?>/>Em Análise</label>
                                        <label><input type="radio" name="analise" value="analisado" <?= $_REQUEST["analise"] == "analisado" ? 'checked="checked"' : '' ?>/>Analisado</label>
                                    </td>
                                </tr>
                            </table>
                            <br />
                        </div>

                        <div style="float: right;">
                            <input type="submit" name="" value="Pesquisar" />
                        </div>
                    </form>
                </td>
            </tr>
        </tbody>
    </table>

<?php

    $municipios = cte_pegarMunicipiosPermitidos();
    $municipios_sql = "'" . implode( "','", $municipios ) . "'";

    $campos = array(  "m.mundescricao", "m.muncod", "d.docdsc", "ed.esddsc" );
    $palavras = array_unique( explode( " ", $_REQUEST['filtro'] ) );
    $filtro = array();
    foreach ( $palavras as $palavra ) {
        if ( empty( $palavra ) || strlen( $palavra ) < 3 ) {
            continue; # ignora palavras vazias
        }
        $a = array();
        foreach ( $campos as $campo ) {
            array_push( $a, "lower({$campo}) like lower('%{$palavra}%')" );
        }
        array_push( $filtro, " ( " . implode( " OR ", $a ) . " ) " );
    }
    
//	$join_acoes = "";
//	if ( $par == "com" ) {
//		$join_acoes = "
//			inner join (
//				select iu2.inuid
//				from cte.instrumentounidade iu2
//				inner join cte.pontuacao p2 on p2.inuid = iu2.inuid
//				inner join cte.acaoindicador ai2 on ai2.ptoid = p2.ptoid
//				group by iu2.inuid
//			) as par on par.inuid = iu.inuid
//		";
//	} else {
//		$sql =
//			"select iu2.inuid
//			from cte.instrumentounidade iu2
//			inner join cte.pontuacao p2 on p2.inuid = iu2.inuid
//			inner join cte.acaoindicador ai2 on ai2.ptoid = p2.ptoid
//			group by iu2.inuid";
//		$instrumentos = $db->carregarColuna( $sql, "inuid" );
//		$instrumentos = $instrumentos ? $instrumentos : array();
//		array_push( $filtro, " ( iu.inuid is null or iu.inuid not in ( '". implode( "','", $instrumentos ) ."' ) ) " );
//		unset( $instrumentos );
//	}
    
    switch ( $_REQUEST["analise"] ) {
        case "naoanalisado":
            array_push( $filtro, " ( subacoes.total > 0 AND subacoesanalisadas.total = 0 ) " );
            break;
        case "emanalise":
            array_push( $filtro, " ( subacoes.total > 0 AND subacoesanalisadas.total < subacoes.total ) " );
            break;
        case "analisado":
            array_push( $filtro, " ( subacoes.total > 0 AND subacoesanalisadas.total = subacoes.total ) " );
            break;
        default:
            break;
    }
    
    if ( $par == "com" ) {
        array_push( $filtro, " coalesce(b.qtd,0) > 0 " );
    } else {
        array_push( $filtro, " coalesce(b.qtd,0) = 0 " );
    }
    
    if ( count( $filtro ) > 0 ) {
        $filtro_sql = " AND " . implode( " AND ", $filtro );
    }
    
    if ( !empty( $_REQUEST['estuf'] ) ) {
        $estado = " and m.estuf = '{$_REQUEST['estuf']}' ";
    }
    
    $join_capitais = "";
    if ( $_REQUEST['capitais'] ) {
        $join_capitais = "inner join territorios.estado e on e.estuf = m.estuf and e.muncodcapital = m.muncod";
    }
    
    $join_grandescidades = "";
    if ( $_REQUEST['grandescidades'] ) {
        $join_grandescidades = "
            inner join territorios.muntipomunicipio mtm on mtm.muncod = m.muncod and mtm.estuf = m.estuf and mtm.tpmid = 1
        ";
    }
    
    $join_ideb = "";
    if ( count( (array) $_REQUEST['ideb'] ) > 0 ) {
        $join_ideb = "inner join territorios.muntipomunicipio mtm2 on mtm2.muncod = m.muncod and mtm2.estuf = m.estuf and mtm2.tpmid in (". implode( ",", $_REQUEST['ideb'] ) .")";
    }

    $sql = sprintf(
        "select distinct
            '<a style=\"margin: 0 -20px 0 20px;\" href=\"brasilpro.php?modulo=lista&acao=M&evento=selecionar&muncod='|| m.muncod ||'\"><img src=\"/imagens/alterar.gif\" border=\"0\" title=\"Selecionar\" /></a>' as acao,
            m.muncod,
            m.mundescricao,
            m.estuf,

            coalesce( ed.esddsc,'Não iniciado' ) as esddsc,
            '<center><span style=\"color:cococo;font-size: 10px;\">' || floor((coalesce(b.qtd,0) * 100)/(a.qtdTotal)) || '%%</span>
            <div style=\"text-align: left; margin-left: 5px; padding: 1px 0 1px 0; height: 6px; max-height: 6px; width: 75px; border: 1px solid #888888; background-color:#dcffdc;\" title=\"' ||  floor((coalesce(b.qtd,0) * 100)/(a.qtdTotal))  || '%%\">
            <div style=\"font-size:4px;width: ' || floor((coalesce(b.qtd,0) * 100)/(a.qtdTotal))  || '%%; height: 6px; max-height: 6px; background-color:#339933;\">
            </div></div></center>' as porcentagem,


            '<center><span style=\"color:cococo;font-size: 10px;\">' || floor((coalesce(subacoesanalisadas.total,0) * 100)/(subacoes.total)) || '%%</span>
            <div style=\"text-align: left; margin-left: 5px; padding: 1px 0 1px 0; height: 6px; max-height: 6px; width: 75px; border: 1px solid #888888; background-color:#dcdcff;\" title=\"' ||  floor((coalesce(subacoesanalisadas.total,0) * 100)/(subacoes.total))  || '%%\">
            <div style=\"font-size:4px;width: ' || floor((coalesce(subacoesanalisadas.total,0) * 100)/(subacoes.total))  || '%%; height: 6px; max-height: 6px; background-color:#333399;\">
            </div></div></center>' as porcentagemsubacao

        from territorios.municipio as m
	        left join cte.instrumentounidade iu ON iu.mun_estuf = m.estuf and iu.muncod = m.muncod  and iu.itrid = ". INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ."
	        left join workflow.documento d on d.docid = iu.docid
	        left join workflow.estadodocumento ed on ed.esdid = d.esdid
	        %s
	        %s
	        %s
	        %s
	        left join cte.indicadorespreenchidos as b on b.inuid=iu.inuid
	        left join cte.indicadorestotais as a on a.itrid = iu.itrid

	        left join cte.subacoestotal as subacoes on subacoes.inuid = iu.inuid
	        left join cte.subacoesanalisadas as subacoesanalisadas on subacoesanalisadas.inuid = iu.inuid
        where m.muncod in ( %s ) %s %s",
        $join_acoes,
        $join_capitais,
        $join_grandescidades,
        $join_ideb,
        $municipios_sql,
        $filtro_sql,
        $estado
    );

 // dbg( $sql,1 );
    
    $cabecalho = array( "Ação", "Código", "Município", "UF", "Situação", "Preenchimento", "Análise" );
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', '', '' );

} // if ( $_REQUEST["acao"] == "M" )
else if ( $_REQUEST["acao"] == "E" ) {
    $unidades     = cte_pegarUfsPermitidas();
    $unidades_sql = "e.estuf='" . implode( "' OR e.estuf='", $unidades ) . "'";
    
    $campos       = array( "e.estdescricao", "d.docdsc", "ed.esddsc" );
    $palavras     = array_unique( explode( " ", $_REQUEST['filtro'] ) );
    $filtro       = array();

    foreach ( $palavras as $palavra ) {
        if ( empty( $palavra ) || strlen( $palavra ) < 3 ) {
            continue; # ignora palavras vazias
        }
        $a = array();
        foreach ( $campos as $campo ) {
            array_push( $a, "lower({$campo}) like lower('%{$palavra}%')" );
        }
        array_push( $filtro, " ( " . implode( " OR ", $a ) . " ) " );
    }
    if ( count( $filtro ) > 0 ) {
        $filtro_sql = " AND " . implode( " AND ", $filtro );
    }
    
    $sql = sprintf(
        "select
            '<a style=\"margin: 0 -20px 0 20px;\" href=\"brasilpro.php?modulo=lista&acao=E&evento=selecionar&estuf='|| e.estuf ||'\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>' as acao,
            e.estuf,
            e.estdescricao,
            coalesce( ed.esddsc,'Não iniciado' ) as esddsc,
            '<center><span style=\"color:cococo;font-size: 10px;\">' || floor((coalesce(b.qtd,0) * 100)/(a.qtdTotal)) || '%%</span><div style=\"text-align: left; margin-left: 5px; padding: 1px 0 1px 0; height: 6px; max-height: 6px; width: 75px; border: 1px solid #888888; background-color:#dcffdc;\" title=\"' ||  floor((coalesce(b.qtd,0) * 100)/(a.qtdTotal))  || '%%\"><div style=\"font-size:4px;width: ' || floor((coalesce(b.qtd,0) * 100)/(a.qtdTotal))  || '%%; height: 6px; max-height: 6px; background-color:#339933;\"></div></div></center>' as porcentagem
        from
            territorios.estado as e
        left join
            cte.instrumentounidade iu ON iu.estuf = e.estuf and iu.itrid = ". INSTRUMENTO_DIAGNOSTICO_ESTADUAL ."
        left join
            workflow.documento d on d.docid = iu.docid
        left join
            workflow.estadodocumento ed on ed.esdid = d.esdid
        left join (
            select
                count(*) as qtd,
                i.inuid
            from
                cte.instrumentounidade i
            inner join
                cte.dimensao d on d.itrid = i.itrid and d.dimstatus = 'A'
            inner join
                cte.areadimensao a on a.dimid = d.dimid and a.ardstatus = 'A'
            inner join
                cte.indicador ind on ind.ardid = a.ardid and ind.indstatus = 'A'
            inner join
                cte.pontuacao p on p.indid = ind.indid and p.inuid=i.inuid and p.ptostatus = 'A'
            inner join
                cte.criterio c on c.crtid = p.crtid
            where
                i.itrid=3
            group by
                i.inuid
        ) as b on b.inuid=iu.inuid
        left join (
            select
                count(*) as qtdTotal,
                i.inuid
            from
                cte.instrumentounidade i
            inner join
                cte.dimensao d on d.itrid = i.itrid and d.dimstatus = 'A'
            inner join
                cte.areadimensao a on a.dimid = d.dimid and a.ardstatus = 'A'
            inner join
                cte.indicador ind on ind.ardid = a.ardid and ind.indstatus = 'A'
            where
                i.itrid=3
            group by
                i.inuid
        ) as a on a.inuid = iu.inuid
        --where e.estuf in ( %s ) %s
        where ( %s ) %s
        order by e.estdescricao
        ",
        $unidades_sql, $filtro_sql,
        $unidades_sql, $filtro_sql
    );

    //echo  "<pre>\n", simec_htmlentities($sql) , "</pre>\n";
    $cabecalho = array("Ação", "Código", "Unidade da Federação", "Situação", "Preenchimento");
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', '', '');
}




