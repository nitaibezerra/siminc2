<?php
include_once APPRAIZ . 'includes/workflow.php';

if ( $_REQUEST['estuf'] && $_REQUEST['evento'] == 'selecionar' ) {
    if( cte_selecionarUf( INSTRUMENTO_DIAGNOSTICO_ESTADUAL, WF_TIPO_BRASILPRO, $_REQUEST['estuf'] ) ) {
        $db->commit();
        header( "Location: ?modulo=principal/monitoramentoInterno/listaSubsFrm&acao=E&estuf=".$_REQUEST['estuf'] );
        exit();
    }
}

$perfis = CTE_PERFIL_MONITORA_SUBACAO;

$ufsPermitidas = cte_pegarUfsPermitidas( $perfis );

$conta = count($ufsPermitidas);

if( $conta == 1 ){
	header( "Location: ?modulo=principal/monitoramentoInterno/listaSubsFrm&acao=E&estuf=".$ufsPermitidas[0] );
	exit();
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
echo '<br />';
$db->cria_aba($abacod_tela, $url, '');

?>    
<table align="center" border="0" class="tabela" bgcolor="#dcdcdc" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td align="center" bgcolor="#dcdcdc" class="TituloTela">
				<?=$titulo_modulo ?>
			</td>
		</tr>
<?php 

	$unidades     = cte_pegarUfsPermitidas( $perfis );
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
    
    $sql = "
    	SELECT
    		DISTINCT
	            '<a style=\"margin: 0 -20px 0 20px;\" href=\"brasilpro.php?modulo=principal/monitoramentoInterno/listaUfs&acao=A&evento=selecionar&estuf='|| e.estuf ||'\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>' as acao,
	            e.estuf,
	            e.estdescricao,
	            '<div class=\"fundo2\" >
					<div class=\"barra12\" >
						<div style=\" text-align:center; position: absolute; width: 100px; height: 12px; padding: 0 0 0 0;  margin-bottom: 1px; \" >
							<div style=\" color: #cococo ; font-size: 10px; max-height: 12px;  \">' || (x.pesototal/x.qtdtotal) || '<span style=\"color: black \" >\%</span></div>
						</div>
						<img class=\" imgBarra2 \" style=\"width:' || (x.pesototal/x.qtdtotal) || '\%;\" src=\"../../imagens/cor1.gif\"/>
					</div>
				 </div>' as concluido
        FROM territorios.estado as e
        LEFT JOIN cte.instrumentounidade   iu ON iu.estuf = e.estuf AND iu.itrid = ". INSTRUMENTO_DIAGNOSTICO_ESTADUAL ."
        LEFT JOIN (
			SELECT
				(SELECT
					SUM(facpeso) as peso
				FROM cte.fasescategoria fc
				INNER JOIN cte.monitoramentosubacoes       ms ON ms.facid = fc.facid AND mosstatus = 'A'
				INNER JOIN cte.categoriasubacao            cs ON cs.sbaid = ms.sbaid AND casexibir = TRUE
				INNER JOIN cte.subacaoindicador            sa ON sa.sbaid = ms.sbaid
				INNER JOIN cte.acaoindicador               ai ON ai.aciid  = sa.aciid 
				INNER JOIN cte.pontuacao                    p ON p.ptoid   = ai.ptoid AND p.ptostatus = 'A'
				INNER JOIN cte.instrumentounidade         inu ON inu.inuid = p.inuid
				WHERE 
					ms.mosfinalizado = 't' AND 
					inu.estuf = y.estuf ) as pesototal,
			
				(SELECT Count(sbaid) FROM (
					SELECT DISTINCT s.sbaid, iu.estuf, iu.itrid, ps.pssano as ano
					FROM cte.subacaoindicador		s
					INNER JOIN cte.acaoindicador		a  ON s.aciid = a.aciid
					INNER JOIN cte.pontuacao		p  ON p.ptostatus = 'A' AND p.ptoid = a.ptoid
					INNER JOIN cte.instrumentounidade	iu ON iu.inuid = p.inuid
					INNER JOIN cte.projetosapesubacao	ps ON ps.sbaid = s.sbaid AND s.frmid IN (17)
				UNION ALL
					SELECT DISTINCT s.sbaid, iu.estuf, iu.itrid, sp.sptano as ano
					FROM cte.subacaoindicador		s
					INNER JOIN cte.acaoindicador		a  ON s.aciid = a.aciid
					INNER JOIN cte.pontuacao		p  ON p.ptostatus = 'A' AND p.ptoid = a.ptoid
					INNER JOIN cte.instrumentounidade	iu ON iu.inuid = p.inuid
					INNER JOIN cte.subacaoparecertecnico	sp ON sp.sbaid = s.sbaid AND sp.ssuid = 3 AND s.frmid IN (13,15)
				) AS FOO WHERE itrid = 3 AND estuf = y.estuf ) as qtdtotal,
				y.estuf
			FROM cte.instrumentounidade y) x ON x.estuf = e.estuf
        WHERE ( {$unidades_sql} ) {$filtro_sql}
        ORDER BY e.estdescricao";
//dbg($sql);
    $cabecalho = array("Ação", "Código", "Unidade da Federação", "Progresso do Monitoramento");
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', '', '');
?>
	</tbody>
</table>
