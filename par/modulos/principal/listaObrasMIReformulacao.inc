<?php
/*** INCLUDES ***/
include_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . 'includes/cabecalho.inc';

/*** INSTANCIA AS DE CLASSES ***/
$obPreObraControle 		= new PreObraControle();
$obEntidadeParControle 	= new EntidadeParControle();

/*** DECLARAÇÃO DE VARIAVEIS E FUNCOES ***/
if( !$_SESSION['par']['inuid'] ){
	echo "<script>
                    alert('Faltam dados na sessão!');
                    history.back(-1);
              </script>";
	die;
}

// verifica se é um estado ou um municipio
if(empty($_SESSION['par']['muncod']) && !empty($_SESSION['par']['estuf'])) {
    $descricao = $obEntidadeParControle->recuperaDescricaoEntidadePar($_SESSION['par']['estuf'], 1);
} else {
    $descricao = $obEntidadeParControle->recuperaDescricaoEntidadePar($_SESSION['par']['muncod'], null);
}

echo "<br>";
monta_titulo( 'Lista de Obras MI passíveis de reformulação', "Para solicitar a reformulação, entre na obra e acesse a aba 'Enviar para Análise' <br>");

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
<tr>
		<td style="color: blue; font-size: 22px">
			<a href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore"> 
				<?php echo $descricao ?>
			</a>
		</td>
		<td style="color: blue; font-size: 12px; text-align:right">
			<a href="par.php?modulo=principal/planoTrabalho&acao=A">Voltar à Árvore</a>
		</td>
	</tr>
</table>

<script>
function abrepopUpPar( preid )
{
	url = 'par.php?modulo=principal/subacaoObras&acao=A&preid=' + preid;
	window.open(url, 'popupAvisoObras', "height=600,width=950,scrollbars=yes,top=50,left=200" );
}

function abrepopUpPac( preid )
{
	url = 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&preid=' + preid;
	window.open(url, 'popupAvisoObras', "height=600,width=950,scrollbars=yes,top=50,left=200" );
}

</script>
<?
if($_SESSION['par']['muncod'])
{
	$muncod = $_SESSION['par']['muncod'];
	$whereEsfera = " AND pro.muncod = '{$muncod}'";
	$whereEsferaObrasMI = "AND pre.muncod  = '{$muncod}'";

}
else
{
	$uf = $_SESSION['par']['estuf'];
	$whereEsfera = " AND pro.estuf = '{$uf}'";
	$whereEsferaObrasMI = "AND pre.estuf =  '{$uf}'";
}

// RECUPERA OBRAS DO PAR
$sql = "
    SELECT 
		distinct
		case when pre.tooid = 1 then
				
				'<img id=\"' || pre.preid || '\" class=\"\" style=\"cursor:pointer\" src=\"../imagens/alterar.gif\" onclick=\"abrepopUpPac(' || pre.preid || ')\">'
				
			else
				'<img id=\"' || pre.preid || '\" class=\"\" style=\"cursor:pointer\" src=\"../imagens/alterar.gif\" onclick=\"abrepopUpPar(' || pre.preid || ')\">'
			end
			as acao,
		 
		pre.preid::text || ' ' as preid,
		o.obrid::text || ' ' as obrid,
		pre.predescricao as nome_obra,
		pto.ptodescricao as tipo_obra,
		esddsc as situacao_obra,
		CASE WHEN doc.tpdid = 37 THEN
		
			CASE WHEN esd.esdid = 1561 THEN
				'Aguardando validação'
			ELSE
				CASE WHEN 
					exists(
						SELECT aed.esdiddestino from workflow.historicodocumento hd
						INNER JOIN workflow.acaoestadodoc aed ON aed.aedid = hd.aedid
						where docid = doc.docid
						and aed.esdiddestino = 1561
						order by htddata desc
						limit 1
					)
				THEN
					'Reprovada'
				ELSE
				'-'
				END
			END
			
			-- 1561 -- Aguardando validacao @todo
			
		WHEN doc.tpdid = 45 THEN
		
			CASE WHEN esd.esdid = 1563 THEN
				'Aguardando validação'
			ELSE
			
				CASE WHEN 
					exists(
						SELECT aed.esdiddestino from workflow.historicodocumento hd
						INNER JOIN workflow.acaoestadodoc aed ON aed.aedid = hd.aedid
						where docid = doc.docid
						and aed.esdiddestino = 1563
						order by htddata desc
						limit 1
					)
				THEN
					'Reprovada'
				ELSE
				'-'
				END
			END
			-- 1563 -- Aguardando validacao @todo
			
		END
		AS situacao_pedido -- @todo atualizar ids
		 
	FROM 
		obras.preobra pre
	INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
	INNER JOIN workflow.documento doc ON pre.docid = doc.docid
	INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid AND esdstatus = 'A'
	INNER JOIN obras2.obras o 
		INNER JOIN obras2.obras_arquivos oa ON oa.obrid = o.obrid AND tpaid = 30 AND oarstatus = 'A'
	on o.preid = pre.preid AND o.obrstatus = 'A'
	
	WHERE 
		pre.ptoid in (43, 42, 44, 45) -- MI
	AND 
		esd.esdid in ( 360, 624, 337, 228, 1561, 1563 ) -- EM REFORMULAÇÃO -- OBRA APROVADA
	AND
		pre.preid  not IN  
		( 
			SELECT distinct o3.preid from obras2.obras o3 
			inner join workflow.documento doc3 ON o3.docid = doc3.docid 
			inner join workflow.estadodocumento esd3 ON esd3.esdid = doc3.esdid AND esd3.esdstatus = 'A'	
	
			WHERE 
			esd3.esdid in ( 
				 691
				,690 
				,769
			)  and preid is not null
			 AND o3.obrstatus = 'A'
		)
	AND
		pre.prestatus = 'A'
    {$whereEsferaObrasMI}
    
    
";

    $cabecalho = array(
    		"Ação",
    		"Preid",
    		"Obrid",
    		"Nome da obra",
			"Tipo da obra",
    		"Situação",
			"Situação do pedido"
    		
    );
    
    
// ver($sql, d);

$db->monta_lista($sql,$cabecalho,50000,5,'N','95%','S');

?>
<div id="divDebug"></div>