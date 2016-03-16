<?php
$_SESSION['inuid'] = cte_pegarInuidDeUf( INSTRUMENTO_DIAGNOSTICO_ESTADUAL, $_REQUEST['estuf'] );
$_SESSION['alt'] = '1';

cte_verificaSessao();

require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";
include_once( APPRAIZ. "brasilpro/classes/EntidadeDetalhe.class.inc" );

$ufsPermitidas = cte_pegarUfsPermitidas();

if( !in_array($_REQUEST['estuf'],$ufsPermitidas) ){
	header('Location: brasilpro.php?modulo=principal/monitoramentoInterno/listaUfs&acao=A');
	exit();
}

$obEntidaDedetalhe = new EntidadeDetalhe();
$estado_documento = wf_pegarEstadoAtual( cte_pegarDocid( $_SESSION['inuid'] ) );
$boPossuiPerfil = cte_possuiPerfilExclusivo( array( CTE_PERFIL_MONITORA_SUBACAO ) );

if( ( $estado_documento['esdid'] == CTE_ESTADO_DIAGNOSTICO || $estado_documento['esdid'] == CTE_ESTADO_PAR ) && $boPossuiPerfil ) {
	if( $obEntidaDedetalhe->existePendenciaEPTPorInuid( $_SESSION["inuid"] ) ){
		echo "<script>location.href = '?modulo=principal/monitoramentoInterno/listaUfs&acao=A';</script>";
	}
}

if ( $_REQUEST['titleFor'] ) {

    $sql = '
    SELECT
        CASE WHEN sa.sbaporescola <> false 
        	THEN \'Por escola\'
        	ELSE \'Global\'
        END as sbaporescola,
        usu.usunome,
        to_char(sa.sbadata,\'dd/mm/YYYY HH24:MI:SS\') as data,
        coalesce(ss.ssudescricao, \'Não informado\') as ssudescricao,
        coalesce(fat.foadsc, \'Não informado\')      as fatendimento,
        coalesce(fex.frmdsc, \'Não informado\')      as frmexecucao
    FROM
        cte.subacaoindicador sa
    LEFT JOIN
        cte.subacaoparecertecnico spt on sa.sbaid = spt.sbaid AND sptano = date_part(\'year\', current_date)
    LEFT JOIN
        cte.statussubacao ss on ss.ssuid = spt.ssuid
    LEFT JOIN
        cte.formaatendimento fat on fat.foaid = sa.foaid 
    LEFT JOIN
        cte.formaexecucao fex on fex.frmid = sa.frmid 
    LEFT JOIN
        seguranca.usuario AS usu ON (sa.usucpf = usu.usucpf)
    WHERE
        sa.sbaid = ' . (integer) $_REQUEST['titleFor'];
    
    if (($dados = $db->pegaLinha($sql)) !== false) {
    	
    	if($_REQUEST['titleFor']){
			$sql = 'SELECT sptano, 
	    	   			   coalesce(ss.ssudescricao, \'Não informado\') as ssudescricao FROM cte.subacaoparecertecnico spt
					LEFT JOIN cte.statussubacao ss on ss.ssuid = spt.ssuid
					WHERE spt.sbaid = ' . (integer) $_REQUEST['titleFor'] .' ORDER BY sptano ';
	    	$arStatus = $db->carregar($sql);
    	}
    	
    	$arStatus = (is_array($arStatus)) ? $arStatus : array();
    	$stStatus = "";
    	
    	foreach ($arStatus as $status){
    		$stStatus .= "&nbsp;&nbsp;&nbsp;".$status['sptano'] .': '. $status['ssudescricao']."<br />"; 
    	}
    	
    	$stStatus = ($stStatus) ? $stStatus : "&nbsp;&nbsp;&nbsp;Não Informado";
    	header("Content-Type: text/html; charset=ISO-8859-1");     	
        echo ''
            ,'Última atualização feita por '   , $dados['usunome']      , ' em ' , $dados['data'] , '<br /><br />'
            ,'<strong>Status: </strong>',  '<br />'
            ,$stStatus,  '<br />'
            ,'<strong>Cronograma: </strong>'        , $dados['sbaporescola'] , '<br />'
            ,'<strong>Forma de execução: </strong>' , $dados['frmexecucao'];
            
    } else {
        echo '<em>Nenhuma atualização realizada.</em>';
    }
    
    die();
}

function exibeErrosSubAcao($erros) { 	

	echo " <script>
			function alterarSubacao( sbaid ){
				".
//	var janela = window.open( \"../brasilpro/brasilpro.php?modulo=principal/par_subacao&acao=A&sbaid=\" + sbaid, 'blank', 'height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
				"janela.focus();
			}
	  	  </script>
		  <div style=\"width : 100%;\">
			<table class=\"tabela\">
			<tr>
			<td colspan='2'>O sistema verificou que alguns dados das seguintes subações não foram preenchidos :</td>
			</tr>";

	foreach($erros as $idsubacao => $dados) {
		echo "<tr style=\"background-color: #d9d9d9;\">";
		echo "<td><img src='/imagens/consultar.gif' onclick='alterarSubacao(". $idsubacao .")'></td>";
		echo "<td><b>". $dados['nome'] ."</b><br />";

		if($dados['parecer']) {
			echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$dados['parecer']."<br />";
		}

		if($dados['PI']) {
			echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$dados['PI']."<br />";
		}

		if($dados['escola']) {
			echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$dados['escola']."<br />";
		}

		if($dados['quantidadesubacao']) {
			echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$dados['quantidadesubacao']."<br />";
		}

		if($dados['descricaosubacao']) {
			echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$dados['descricaosubacao']."<br />";
		}

		if(($dados['descricaoitemcomposicao']) ||($dados['itemcomposicao']) ) {

			echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<i>Erros nos itens de composição:</i><br />";

			if($dados['descricaoitemcomposicao']) {
					echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Existem itens de composição sem identificação. (Verifique em  Editar / Inserir Itens de Composição)</br>";
			}

			if($dados['itemcomposicaovalor']) {

				if(!is_array($dados['itemcomposicaovalor'])) {
					echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ". $dados['itemcomposicaovalor'] ." ";
				} else {

					echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;O valor unitario dos itens abaixo não podem ser Zero:</br>";

					foreach($dados['itemcomposicaovalor'] as $item) {
						echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$item."<br />";
					}
				}
				echo"<br />";
			}

			if($dados['itemcomposicao']) {

				if(!is_array($dados['itemcomposicao'])) {
					echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ". $dados['itemcomposicao'] ."<br />";
				} else {
					
					echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;A quantidade dos itens abaixo não podem ser Zero:</br>";

					foreach($dados['itemcomposicao'] as $item) {
						echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$item."<br />";
					}
				}
				echo"<br />";
			}
		}

		if($dados['beneficiarios']) {

			echo "<br />";

			if(!is_array($dados['beneficiarios'])) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ". $dados['beneficiarios'] ."<br />";
			} else {

				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<i>Erros nos beneficiários:</i><br />";

				foreach($dados['beneficiarios'] as $benef) {
					echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$benef."<br />";
				}
			}
		}
		echo "</td></tr>";
	}
	echo "</table>";	
}

include_once APPRAIZ . 'includes/workflow.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
if( $estado_documento['esdid'] != CTE_ESTADO_DIAGNOSTICO ) {

	$db->cria_aba( $abacod_tela, $url, '' );
	cte_montaTitulo($titulo_modulo. '<br>', '<img src="/imagens/atencao.png" align="middle"/> Itens com pendências
									&nbsp;&nbsp;&nbsp;
									<img src="/imagens/email.gif" align="middle"/> Dúvidas'
										);
} else {
	$db->cria_aba( $abacod_tela, $url, '' );
	cte_montaTitulo( $titulo_modulo, '' );
}

$itrid = cte_pegarItrid( $_SESSION['inuid'] );

function delimitador($texto){
	if(strlen($texto) > 120){
		$texto = substr($texto,0,120).'...';
	}
	return $texto;
}

$bloqueioSys = 'f';
$sql = "SELECT 
			madbloqueio,
			madestados,
			madalerta
  		FROM 
  			cte.monitoramentoadministracao
  		WHERE
  			madid = 1
  		LIMIT 1";

$dados = $db->pegaLinha($sql);
$bloc 	 = $dados['madbloqueio'];
$blocEst = explode(",",$dados['madestados']);
if( $bloc == 't' && in_array($_REQUEST['estuf'],$blocEst) ){
	$bloqueioSys = 't';
}

$bloqueio = ($bloqueioSys == 't') ? '<img src="/imagens/cadiado.png" align="middle"/><br /> Sistema Bloqueado.<br />' : '';

?>

<script language="javascript" type="text/javascript" src="../includes/dtree/dtree.js"></script>
<script type="text/javascript" src="../includes/remedial.js"></script>
<script type="text/javascript" src="../includes/superTitle.js"></script>
<script type="text/javascript" src="../../includes/LyteBox/lytebox.js"></script>
<script src="../includes/prototype.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/dtree/dtree.css">
<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
<link rel="stylesheet" type="text/css" href="../../includes/LyteBox/lytebox.css"/>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<colgroup>
		<col />
	</colgroup>
	<tbody>
	<?php if( $dados['madalerta'] ){?>
		<tr>
			<td class='SubTituloDireita' width="10%"></td>
			<td align="center">
				<label style="color: red;">
					<?=$bloqueio ?>
					Aviso: <?=$dados['madalerta'] ?>
				</label>
			</td>
		</tr>
	<?php }?>
	<?php if( cte_possuiPerfil(array(CTE_PERFIL_SUPER_USUARIO,CTE_PERFIL_ADMINISTRADOR, CTE_PERFIL_ADMINISTRATOR_TEMP )) ){?>
		<tr id="anosubacoes" >
			<td class='SubTituloDireita' width="10%">
				UF:
			</td>
			<td>
				<?php 
				$stSql = " SELECT 
								estuf as codigo,
								estuf as descricao
							FROM 
								territorios.estado
							ORDER BY
								estuf ";
				
				$db->monta_combo( 'estuf', $stSql, 'S', 'Todos','','','','243','N','estuf','',$_REQUEST['estuf']);
				?>
			</td>
		</tr>
	<?php }?>
		<tr id="anosubacoes" >
			<td class='SubTituloDireita' width="10%">
				Ano:
			</td>
			<td>
				<?php 
				$ano = $_REQUEST['ano'] ? $_REQUEST['ano'] : ''; 
				?>
				<select id="ano" class="CampoEstilo" name="ano" style="width:60px;">
					<option value="<?=$ano?>"><?=$ano ?></option>
					<option value=" --- "> --- </option>
					<?php for($x=2008;$x<=date('Y');$x++): ?>
						<option value="<?php echo $x ?>"><?php echo $x ?></option>
					<?php endfor; ?>
				</select>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td class='SubTituloDireita' width="10%">
				Convênio:
			</td>
			<td>
				<?php
					
					$convenio = $_REQUEST['cmoid'] ? $_REQUEST['cmoid'] : '' ;
				
					$sql = "SELECT 
								cmoid   as codigo, 
								cmodsc || '/' || cmoano || ' - ' || cmosigla as descricao 
							FROM 
								cte.conveniomonitoramento 
							WHERE
								estuf = '{$_REQUEST['estuf']}'
							ORDER BY 
								cmodsc, 
								cmoano asc";
					
		            $db->monta_combo( 'convenio', $sql, 'S', 'Selecione', '', '','','200','N','convenio' );
		            
				?>
			</td>
		</tr>
		<tr>
			<td class='SubTituloDireita' width="10%">
			</td>
			<td>
				<input type="button" id="btnListar" name="btnListar" value="Listar Subações" onclick="listarSubacoes()"/>
			</td>
		</tr>
		<tr>
			<td class='SubTituloDireita' width="10%">
			</td>
			<td align="center">
				<div style="width: 20%; font-size: 11px;" onclick="window.open('/brasilpro/arquivos/guia_pratico_SMBP.pdf');" align="center">
					<img src="/imagens/pdf_adobe.jpg" ><br>
					Em caso de dúvida clique aqui para baixar o <br>Guia Prático do Monitoramento.
				</div>
			</td>
		</tr>
		<tr>
			<td style="padding: 15px; background-color: #fafafa; color: #404040; vertical-align: top;" colspan="4">
				<div id="bloco" style="overflow: hidden;">
			<? 
			if($_REQUEST['exibirerros'] && $_SESSION['validaErro']) {
				if(!$_SESSION['validaErroAtualizado']) {
//					echo "<script> 
//							location.href = '/brasilpro/brasilpro.php?modulo=principal/enviar&acao=A';
//		  				  </script>";
					exit;
				}
				exibeErrosSubAcao($_SESSION['validaErro']);
				$_SESSION['validaErroAtualizado'] = false;
			} else {	
			?>
					<p>
						<a href="javascript: arvore.openAll();">Abrir Todos</a>
							&nbsp;|&nbsp; 
						<a href="javascript: arvore.closeAll();">Fechar Todos</a>
					</p>
					<div id="_arvore"></div>
			<? 
			} 
			?>
				</div>
			</td>
			<td style="background-color: #fafafa; color: #404040; width: 1px;" valign="top">
			</td>
		</tr>
	</tbody>
</table>

<? 

?>

<script type="text/javascript">
	var estuf = '<?=$_REQUEST['estuf'] ?>';
<?
if(!$_REQUEST['exibirerros'] && $_REQUEST['ano'] ) { 
	
//SQL's Subações
	if( $_REQUEST['cmoid'] != "" ){
		$convenio = "AND cs.cmoid = {$_REQUEST['cmoid']}" ;
		$varConvenio = "cs.cmoid, cm.cmodsc || '/' || cm.cmoano as convenio,";
		$joinConvenio = "INNER JOIN cte.conveniosubacao       cs ON cs.sbaid = sa.sbaid AND csustatus = 'A'
						 INNER JOIN cte.conveniomonitoramento cm ON cm.cmoid = cs.cmoid";
		$leftParecer = " = cs.csuid";
		$convParecer = " = {$_REQUEST['cmoid']}";
	}else{
		$convenio = "";
		$varConvenio = "";
		$joinConvenio = "";
		$leftParecer = " is null";
		$convParecer = " is null";
	}
	
	$sql_subacao_af = "SELECT DISTINCT
							sa.sbaid, 
							sa.sbadsc, 
							sa.sbaordem,
							sa.sbaidpai,
							ss.ssudescricao,
						    dim.dimcod,
						    are.ardcod,
						    ind.indcod,
							--ps.prsnumconvsape AS prsnumconvs,
							mns2.possuiduv,
							{$varConvenio}
							qtdcs.conveniado,
							mns.mosalterado
							--CASE WHEN mns.mosaprovado = 'f'
							--	THEN mns.mosaprovado
							--	ELSE 't'
							--END as mosaprovado
						FROM 
							cte.subacaoindicador sa 
						INNER JOIN cte.categoriasubacao     	  cts ON cts.sbaid = sa.sbaid AND casexibir = TRUE
						INNER JOIN cte.acaoindicador               ai ON ai.aciid  = sa.aciid 
						INNER JOIN cte.pontuacao                    p ON p.ptoid   = ai.ptoid AND p.ptostatus = 'A'
						INNER JOIN cte.indicador                  ind ON ind.indid = p.indid
						INNER JOIN cte.areadimensao               are ON are.ardid = ind.ardid
						INNER JOIN cte.dimensao                   dim ON dim.dimid = are.dimid
						INNER JOIN cte.instrumentounidade         inu ON inu.inuid = p.inuid
						--INNER JOIN cte.projetosape                 ps ON ps.inuid  = p.inuid --AND ps.prsnumconvsape IS NOT NULL 
						--INNER JOIN cte.projetosapesubacao         pss ON pss.sbaid = sa.sbaid AND pss.pssano = {$_REQUEST['ano']}
						INNER JOIN cte.subacaoparecertecnico      spt ON spt.sbaid = sa.sbaid AND cast(spt.sptano AS varchar) = '{$_REQUEST['ano']}' AND spt.ssuid = 7
						LEFT  JOIN cte.statussubacao               ss ON ss.ssuid  = spt.ssuid
						LEFT  JOIN seguranca.usuario              usu ON sa.usucpf = usu.usucpf
						LEFT  JOIN cte.formaexecucao              fex ON fex.frmid = sa.frmid 
						{$joinConvenio}
						LEFT  JOIN (SELECT
										count(*) as conveniado,
										sbaid
									FROM
										cte.conveniosubacao
									WHERE  
										csustatus = 'A'
									GROUP BY
										sbaid )                 qtdcs ON qtdcs.sbaid = sa.sbaid
						LEFT  JOIN (SELECT DISTINCT 
										ms.sbaid,
										csuid, 
										mosano,
										mosalterado
									FROM cte.monitoramentosubacoes ms
									INNER JOIN cte.categoriasubacao cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
									WHERE ms.mosstatus = 'A' ) as mns ON mns.sbaid = sa.sbaid AND mns.mosano = spt.sptano AND mns.csuid  {$leftParecer}
						LEFT  JOIN (SELECT DISTINCT 
										COUNT(mduid) as possuiduv,
										ms.sbaid,
										csuid,
										mosano
									FROM 
										cte.monitoramentoduvidas md
									INNER JOIN cte.monitoramentosubacoes ms ON ms.mosid = md.mosid AND mosstatus = 'A'
									INNER JOIN cte.categoriasubacao     cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
									WHERE 
										md.mdusituacao is not null
									GROUP BY ms.sbaid, csuid, mosano, mosalterado ) as mns2 ON mns2.sbaid = sa.sbaid AND mns2.mosano = spt.sptano AND mns2.csuid {$leftParecer}
						WHERE sa.frmid = 17
							AND inu.estuf = '{$_REQUEST['estuf']}'
							AND sa.sbaidpai is null
							AND inu.itrid = 3 
							{$convenio}
						ORDER BY dim.dimcod,are.ardcod,ind.indcod,sa.sbaordem,sa.sbadsc";
	
	$subacoes_af = $db->carregar( $sql_subacao_af );
	
	if ( !$subacoes_af ) {
		$subacoes_af = array();
	}
	
	$sql_subacao_at = "SELECT
							DISTINCT
								sa.sbaid, 
								sa.sbadsc, 
								sa.sbaordem,
								sa.sbaidpai,
								ss.ssudescricao,
								dim.dimcod,
							    are.ardcod,
							    ind.indcod,
							   	mns2.possuiduv,
								qtdcs.conveniado,
								mns.mosalterado,
								{$varConvenio}
							    --CASE WHEN mns.mosaprovado = 'f'
								--	THEN mns.mosaprovado
								--	ELSE 't'
								--END as mosaprovado,
								(SELECT
									SUM(facpeso) as peso
								FROM cte.fasescategoria fc
								INNER JOIN cte.monitoramentosubacoes       ms ON ms.facid = fc.facid AND mosstatus = 'A'
								INNER JOIN cte.categoriasubacao     cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
								INNER JOIN cte.subacaoindicador            sa ON sa.sbaid = ms.sbaid
								INNER JOIN cte.acaoindicador               ai ON ai.aciid  = sa.aciid 
								INNER JOIN cte.pontuacao                    p ON p.ptoid   = ai.ptoid AND p.ptostatus = 'A'
								INNER JOIN cte.instrumentounidade         inu ON inu.inuid = p.inuid
								WHERE 
									ms.mosfinalizado = 't' AND
									sa.frmid = 15 AND 
									inu.estuf = '{$_REQUEST['estuf']}' and ms.mosano = {$_REQUEST['ano']}) as pesototal
						FROM 
							cte.subacaoindicador sa
						INNER JOIN cte.categoriasubacao     	  cts ON cts.sbaid = sa.sbaid AND cts.casexibir = TRUE 
						INNER JOIN cte.acaoindicador               ai ON ai.aciid  = sa.aciid 
						INNER JOIN cte.pontuacao                    p ON p.ptoid   = ai.ptoid AND p.ptostatus = 'A'
						INNER JOIN cte.indicador                  ind ON ind.indid = p.indid
						INNER JOIN cte.areadimensao               are ON are.ardid = ind.ardid
						INNER JOIN cte.dimensao                   dim ON dim.dimid = are.dimid
						INNER JOIN cte.instrumentounidade         inu ON inu.inuid = p.inuid
						INNER  JOIN cte.subacaoparecertecnico      spt ON spt.sbaid = sa.sbaid AND  
																		 spt.ssuid = 3 AND 
																		 spt.sptano = {$_REQUEST['ano']}
						LEFT  JOIN cte.statussubacao               ss ON ss.ssuid  = spt.ssuid
						LEFT  JOIN seguranca.usuario              usu ON sa.usucpf = usu.usucpf
						LEFT  JOIN cte.formaexecucao              fex ON fex.frmid = sa.frmid
						{$joinConvenio}
						LEFT  JOIN (SELECT
										count(*) as conveniado,
										sbaid
									FROM
										cte.conveniosubacao
									WHERE	
										csustatus = 'A'
									GROUP BY
										sbaid )                 qtdcs ON qtdcs.sbaid = sa.sbaid
						LEFT  JOIN (SELECT DISTINCT 
										ms.sbaid,
										csuid, 
										mosano,
										mosalterado
									FROM cte.monitoramentosubacoes ms
									INNER JOIN cte.categoriasubacao     cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
									WHERE mosstatus = 'A' ) as mns ON mns.sbaid = sa.sbaid AND mns.mosano = spt.sptano AND mns.csuid  {$leftParecer}
						LEFT  JOIN (SELECT DISTINCT 
										COUNT(*) as possuiduv,
										ms.sbaid,
										csuid,
										mosano
									FROM 
										cte.monitoramentoduvidas md
									INNER JOIN cte.monitoramentosubacoes ms ON ms.mosid = md.mosid AND mosstatus = 'A'
									INNER JOIN cte.categoriasubacao     cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
									WHERE 
										mdusituacao is not null
									GROUP BY ms.sbaid, csuid, mosano,mosalterado ) as mns2 ON mns2.sbaid = sa.sbaid AND mns2.mosano = spt.sptano AND mns2.csuid {$leftParecer}
	 					WHERE sa.frmid = 15 
							AND inu.estuf = '{$_REQUEST['estuf']}'
							AND sa.sbaidpai is null
							AND inu.itrid = 3
							{$convenio}
						ORDER BY dim.dimcod,are.ardcod,ind.indcod,sa.sbaordem, sa.sbadsc";
	
	$subacoes_at = $db->carregar( $sql_subacao_at );
	
	if ( !$subacoes_at ) {
		$subacoes_at = array();
	}
	
	$sql_subacao_ede = "SELECT DISTINCT
							sa.sbaid, 
							sa.sbadsc, 
							sa.sbaordem,
							sa.sbaidpai,
							ss.ssudescricao,
							dim.dimcod,
						    are.ardcod,
						    ind.indcod,
						    mns2.possuiduv,
							qtdcs.conveniado,
							mns.mosalterado,
							{$varConvenio}
						    --CASE WHEN mns.mosaprovado = 'f'
							--	THEN mns.mosaprovado
							--	ELSE 't'
							--END as mosaprovado,
							(SELECT 
								SUM(facpeso) as peso
							FROM cte.fasescategoria fc
							INNER JOIN cte.monitoramentosubacoes       ms ON ms.facid = fc.facid AND mosstatus = 'A'
							INNER JOIN cte.categoriasubacao           cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
							INNER JOIN cte.subacaoindicador            sa ON sa.sbaid = ms.sbaid
							INNER JOIN cte.acaoindicador               ai ON ai.aciid  = sa.aciid 
							INNER JOIN cte.pontuacao                    p ON p.ptoid   = ai.ptoid AND p.ptostatus = 'A'
							INNER JOIN cte.instrumentounidade         inu ON inu.inuid = p.inuid
							WHERE 
								ms.mosfinalizado = 't' AND
								sa.frmid = 13 AND 
								inu.estuf = '{$_REQUEST['estuf']}' and ms.mosano = {$_REQUEST['ano']} ) as pesototal
						FROM 
							cte.subacaoindicador sa 
						INNER JOIN cte.categoriasubacao     	  cts ON cts.sbaid = sa.sbaid AND cts.casexibir = TRUE
						INNER JOIN cte.acaoindicador               ai ON ai.aciid  = sa.aciid 
						INNER JOIN cte.pontuacao                    p ON p.ptoid   = ai.ptoid AND p.ptostatus = 'A'
						INNER JOIN cte.indicador                  ind ON ind.indid = p.indid
						INNER JOIN cte.areadimensao               are ON are.ardid = ind.ardid
						INNER JOIN cte.dimensao                   dim ON dim.dimid = are.dimid
						INNER JOIN cte.instrumentounidade         inu ON inu.inuid = p.inuid
						INNER  JOIN cte.subacaoparecertecnico      spt ON spt.sbaid = sa.sbaid AND  
																		 spt.ssuid = 3 AND 
																		 spt.sptano = {$_REQUEST['ano']}
						LEFT  JOIN cte.statussubacao               ss ON ss.ssuid  = spt.ssuid
						LEFT  JOIN seguranca.usuario              usu ON sa.usucpf = usu.usucpf
						LEFT  JOIN cte.formaexecucao              fex ON fex.frmid = sa.frmid
						{$joinConvenio}
						LEFT  JOIN (SELECT
										count(*) as conveniado,
										sbaid
									FROM
										cte.conveniosubacao
									WHERE
										csustatus = 'A'
									GROUP BY
										sbaid )                 qtdcs ON qtdcs.sbaid = sa.sbaid
						LEFT  JOIN (SELECT DISTINCT 
										ms.sbaid,
										csuid, 
										mosano,
										mosalterado
									FROM cte.monitoramentosubacoes ms 
									INNER JOIN cte.categoriasubacao     cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
									WHERE mosstatus = 'A' ) as mns ON mns.sbaid = sa.sbaid AND mns.mosano = spt.sptano AND mns.csuid  {$leftParecer}
						LEFT  JOIN (SELECT DISTINCT 
										COUNT(*) as possuiduv,
										ms.sbaid,
										csuid,
										mosano
									FROM 
										cte.monitoramentoduvidas md
									INNER JOIN cte.monitoramentosubacoes ms ON ms.mosid = md.mosid AND mosstatus = 'A'
									INNER JOIN cte.categoriasubacao     cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
									WHERE 
										mdusituacao is not null
									GROUP BY ms.sbaid, csuid, mosano,mosalterado ) as mns2 ON mns2.sbaid = sa.sbaid AND mns2.mosano = spt.sptano AND mns2.csuid {$leftParecer}
						WHERE sa.frmid = 13 
							AND inu.estuf = '{$_REQUEST['estuf']}'
							AND sa.sbaidpai is null
							AND inu.itrid = 3
							{$convenio}
						ORDER BY dim.dimcod,are.ardcod,ind.indcod,sa.sbaordem, sa.sbadsc";

	$subacoes_ede = $db->carregar( $sql_subacao_ede );
	
	if ( !$subacoes_ede ) {
		$subacoes_ede = array();
	}
	
	$sbaidsAditivos = array() ;
	
	foreach($subacoes_af as $subacao){    
	    
	    if($subacao['sbaidpai'] != '' || $subacao['sbaidpai'] != null){
	        $sbaidsAditivos[] = $subacao["sbaidpai"];        
	    }
	}
	
	foreach($subacoes_at as $subacao){    
	    
	    if($subacao['sbaidpai'] != '' || $subacao['sbaidpai'] != null){
	        $sbaidsAditivos[] = $subacao["sbaidpai"];        
	    }
	}
	
	foreach($subacoes_ede as $subacao){    
	    
	    if($subacao['sbaidpai'] != '' || $subacao['sbaidpai'] != null){
	        $sbaidsAditivos[] = $subacao["sbaidpai"];        
	    }
	}
?>
/*
	Tudo que estava cte foi mudado para brasilpro
*/

	function monitorarSubacao( sbaid, csuid, cmoid, mosalterado ){
		var ano = $('ano').value;
		var janela = window.open( "?modulo=principal/monitoramentoInterno/monitoraSubacao&acao=A&sbaid="+ sbaid+"&csuid="+csuid+"&cmoid="+cmoid+"&estuf="+estuf+"&ano="+ano+"&mosalterado="+mosalterado, 'alterarSubacao', 'height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
	
		janela.focus();
	
	}
	
    var u='/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/listaSubsFrm&acao=E&titleFor=';

    arvore = new dTree( "arvore" );
	arvore.config.folderLinks = true;
	arvore.config.useIcons = true;
	arvore.config.useCookies = true; 
<?php

	$pimp = "&nbsp;&nbsp;".
			"&nbsp;&nbsp;".
			"&nbsp;&nbsp;".
	        "&nbsp;&nbsp;".
	        "&nbsp;&nbsp;".
	        "&nbsp;&nbsp;";
	
	//Montando Arvore
	$sql = "SELECT sbaid FROM (
				SELECT DISTINCT s.sbaid, iu.estuf, iu.itrid, ps.pssano as ano
				FROM cte.subacaoindicador			 s
				INNER JOIN cte.acaoindicador		 a  ON s.aciid = a.aciid
				INNER JOIN cte.pontuacao			 p  ON p.ptostatus = 'A' AND p.ptoid = a.ptoid
				INNER JOIN cte.instrumentounidade	 iu ON iu.inuid = p.inuid
				INNER JOIN cte.projetosapesubacao	 ps ON ps.sbaid = s.sbaid AND s.frmid IN (17)
			UNION ALL
				SELECT DISTINCT s.sbaid, iu.estuf, iu.itrid, sp.sptano as ano
				FROM cte.subacaoindicador			 s
				INNER JOIN cte.acaoindicador		 a  ON s.aciid = a.aciid
				INNER JOIN cte.pontuacao			 p  ON p.ptostatus = 'A' AND p.ptoid = a.ptoid
				INNER JOIN cte.instrumentounidade	 iu ON iu.inuid = p.inuid
				INNER JOIN cte.subacaoparecertecnico sp ON sp.sbaid = s.sbaid AND sp.ssuid = 3 AND s.frmid IN (13,15)
			) AS FOO
			WHERE itrid = 3
			AND estuf = '{$_REQUEST['estuf']}'
			AND ano = {$_REQUEST['ano']}";
	$subacoes = $db->carregar($sql);
	$finalizado = 0;
	$TotalSubacoes = 0;
	if($subacoes){
		foreach( $subacoes as $subacao ){
			
			$sql = "SELECT 
						fc.facpeso, 
						ms.mosfinalizado,
						(SELECT
							count(ms.sbaid)
						 FROM 
						 	cte.monitoramentosubacoes ms
						 INNER JOIN cte.categoriasubacao cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
						 LEFT JOIN cte.conveniosubacao cs ON cs.sbaid = ms.sbaid AND csustatus = 'A'
						 WHERE
						 	mosstatus = 'A' AND ms.sbaid  = ".$subacao['sbaid'].") as qtdmon
					FROM 
						cte.monitoramentosubacoes ms
					INNER JOIN cte.fasescategoria    fc ON fc.facid = ms.facid
					INNER JOIN cte.categoriasubacao cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
					WHERE 
						ms.mosano = ".$_REQUEST['ano']." AND 
						mosstatus = 'A' AND
						ms.sbaid  = ".$subacao['sbaid'];
			
			$facpeso = $db->carregar($sql);
			
			if($facpeso){
				foreach( $facpeso as $peso ){
					if ( $peso['mosfinalizado'] == 't'){
						$finalizado = $finalizado+$peso['facpeso'];
					}
				}
			}
			
			$sqlQtd = "(SELECT DISTINCT
							ms.sbaid
						FROM
							cte.monitoramentosubacoes ms
						INNER JOIN cte.categoriasubacao cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
						WHERE
							ms.mosano = ".$_REQUEST['ano']." AND 
							mosstatus = 'A' AND
							ms.sbaid  = ".$subacao['sbaid']." AND csuid is null)
						UNION ALL
					   (SELECT 
							cs.sbaid
						FROM
							cte.conveniosubacao  cs
						INNER JOIN cte.subacaoparecertecnico spt ON spt.sbaid  = cs.sbaid AND
															 		spt.sptano = {$_REQUEST['ano']}
						INNER JOIN cte.categoriasubacao cts ON cts.sbaid = cs.sbaid AND casexibir = TRUE
						WHERE
							cs.sbaid  = ".$subacao['sbaid']." AND csustatus = 'A')";
			
			$dados = $db->carregar($sqlQtd);
			
		$TotalSubacoes = $TotalSubacoes + count($dados);
		}
	}
	
	
	if ($TotalSubacoes != 0){
		$concluido = ($finalizado/$TotalSubacoes);
	}else{
		$concluido = 0;
	}
	
	if($concluido  == 49 || $concluido  == 50  || $concluido  == 51 ){
		$cor 	= "#FFFFFF";
		$cor1 	= "black";
	}else if($concluido  < 48){
		$cor 	= "#cococo";
		$cor1 	= "black";
	}else if ($concluido  > 51 ){
		$cor 	= "#FFFFFF";
		$cor1 	= "#FFFFFF";
	}
				
	if($concluido > 100){
		$corBarra = 100;
	}else{
		$corBarra = $concluido;
	}
		
	$barra 	=  '';
	$barra .= 	  '<div class="fundo" >'.
						'<div class="barra1" >'.
							'<div style=" text-align:center; position: absolute; width: 40px; height: 8px; padding: 0 0 0 0;  margin-bottom: 1px; " >'.
								'<div style=" color:'.$cor.'; font-size: 8px; max-height: 8px;  ">'.number_format($concluido, 2, ',', '.').'<span style="color:'.$cor1.'" >%</span></div>'.
							'</div>'.
							'<img class="imgBarra" style="width: '.$corBarra.'%;" src="../../imagens/cor1.gif"/>'.
						'</div>'.
					'</div>'.
				'';
	unset($concluido);
	$sql = sprintf( "select itrdsc from cte.instrumento where itrid = %d", $itrid );
	$instrumento = $db->pegaUm( $sql );
	
	echo "arvore.add( 0, -1, '". $instrumento.$barra.$pimp ."' );\n";
	
	if ( true ){
		
		//============= Assistência Financeira =============
		$concluido = 0;
		
		$sqlVlr = " SELECT
						ms.sbaid,
						ms.csuid,
						SUM(facpeso) as peso
					FROM cte.fasescategoria fc
					INNER JOIN cte.monitoramentosubacoes       ms ON ms.facid = fc.facid AND  mosstatus = 'A'
					INNER JOIN cte.categoriasubacao           cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
					INNER JOIN cte.subacaoindicador            sa ON sa.sbaid = ms.sbaid
					INNER JOIN cte.acaoindicador               ai ON ai.aciid  = sa.aciid 
					INNER JOIN cte.pontuacao                    p ON p.ptoid   = ai.ptoid AND p.ptostatus = 'A'
					INNER JOIN cte.instrumentounidade         inu ON inu.inuid = p.inuid
					WHERE 
						ms.mosfinalizado = 't' AND
						sa.frmid = 17 AND 
						inu.estuf = '{$_REQUEST['estuf']}' AND 
						ms.mosano = {$_REQUEST['ano']}
					GROUP BY
						ms.sbaid,ms.csuid";
		
		$dados = $db->carregar($sqlVlr);
		
		$sbaid = 0;
		$csuid = 0;
		unset($vlrTotal);
		if(is_array($dados)){
			foreach($dados as $valor ){
				if( !($sbaid == $valor['sbaid']) ){
					$sbaid    = $valor['sbaid'];
					$scuid    = $valor['csuid'];
					$vlrTotal = $vlrTotal+$valor['peso'];
				}
			}
		}
		
		$sqlQtd = "(SELECT DISTINCT
						sa.sbaid 
					FROM cte.subacaoindicador sa
					LEFT  JOIN cte.monitoramentosubacoes       ms ON sa.sbaid = ms.sbaid AND mosstatus = 'A'
					INNER JOIN cte.categoriasubacao           cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
					LEFT  JOIN cte.conveniosubacao             cs ON cs.sbaid  = sa.sbaid AND csustatus = 'A'
					INNER JOIN cte.acaoindicador               ai ON ai.aciid  = sa.aciid 
					INNER JOIN cte.pontuacao                    p ON p.ptoid   = ai.ptoid AND p.ptostatus = 'A'
					INNER JOIN cte.instrumentounidade         inu ON inu.inuid = p.inuid
					--INNER JOIN cte.projetosapesubacao         pss ON pss.sbaid = sa.sbaid AND pss.pssano = {$_REQUEST['ano']}
					INNER JOIN cte.subacaoparecertecnico      spt ON spt.sbaid = sa.sbaid AND cast(spt.sptano AS varchar) = '{$_REQUEST['ano']}' AND spt.ssuid = 7
					WHERE 
						sa.frmid = 17 AND 
						inu.estuf = '{$_REQUEST['estuf']}'  AND ms.csuid is null AND cs.sbaid is null)
					UNION ALL
				   (SELECT
				   		sa.sbaid
					FROM cte.subacaoindicador sa
					INNER JOIN cte.acaoindicador               ai ON ai.aciid  = sa.aciid 
					INNER JOIN cte.pontuacao                    p ON p.ptoid   = ai.ptoid AND p.ptostatus = 'A'
					INNER JOIN cte.instrumentounidade         inu ON inu.inuid = p.inuid
					--INNER JOIN cte.projetosapesubacao         pss ON pss.sbaid = sa.sbaid AND pss.pssano = {$_REQUEST['ano']}
					INNER JOIN cte.subacaoparecertecnico      spt ON spt.sbaid = sa.sbaid AND cast(spt.sptano AS varchar) = '{$_REQUEST['ano']}' AND spt.ssuid = 7
					INNER JOIN cte.conveniosubacao             cs ON cs.sbaid  = sa.sbaid AND csustatus = 'A'
					WHERE 
						sa.frmid = 17 AND 
						inu.estuf = '{$_REQUEST['estuf']}')";
		
		$dados = $db->carregar($sqlQtd);
		
		$qtd = count($dados);
		
		if( $qtd > 1 ){
			$concluido = $vlrTotal/$qtd;
		}
						
		if($concluido  == 49 || $concluido  == 50  || $concluido  == 51 ){
			$cor 	= "#FFFFFF";
			$cor1 	= "black";
		}else if($concluido  < 48){
			$cor 	= "#cococo";
			$cor1 	= "black";
		}else if ($concluido  > 51 ){
			$cor 	= "#FFFFFF";
			$cor1 	= "#FFFFFF";
		}
					
		if($concluido > 100){
			$corBarra = 100;
		}else{
			$corBarra = $concluido;
		}
		
		$barra 	=  '';
		$barra .= 	  '<div class="fundo" >'.
							'<div class="barra1" >'.
								'<div style=" text-align:center; position: absolute; width: 40px; height: 8px; padding: 0 0 0 0;  margin-bottom: 1px; " >'.
									'<div style=" color:'.$cor.'; font-size: 8px; max-height: 8px;  ">'.number_format($concluido, 2, ',', '.').'<span style="color:'.$cor1.'" >%</span></div>'.
								'</div>'.
								'<img class="imgBarra" style="width: '.$corBarra.'%;" src="../../imagens/cor1.gif"/>'.
							'</div>'.
						'</div>'.
					'';
		unset($concluido);
		echo "arvore.add( 1, 0, '<b>Assistência Financeira ".$barra.$pimp."</b>', '', '', '', '../includes/dtree/img/globe.gif', '../includes/dtree/img/globe.gif' );\n";
		
		unset ($barra);
		
		$barra 	=  '';
		
		foreach( $subacoes_af as $subacao ){ 
		
			$barra = '';
			
			$sql = "SELECT 
						fc.facpeso, 
						ms.mosfinalizado
					FROM 
						cte.monitoramentosubacoes ms
					INNER JOIN cte.fasescategoria    fc ON fc.facid = ms.facid
					INNER JOIN cte.categoriasubacao cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
					WHERE 
						ms.mosano = ".$_REQUEST['ano']." AND 
						mosstatus = 'A' AND
						ms.sbaid  = ".$subacao['sbaid'];
			
			$facpeso = $db->carregar($sql);
			
			if($facpeso){
				
				$concluido = 0;
				$r = 0;
				foreach( $facpeso as $peso ){
					if ( $peso['mosfinalizado'] == 't'){
						$concluido = $concluido+$peso['facpeso'];
					}
					$r++;
				}
			}else{
				$concluido = 0;
			}
			
			$sqlQtd = "(SELECT DISTINCT
							ms.sbaid
						 FROM
						 	cte.monitoramentosubacoes ms
						 INNER JOIN cte.categoriasubacao cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
						 WHERE
						 	ms.sbaid = ".$subacao['sbaid']. " AND mosstatus = 'A' AND csuid is null) 
						 UNION ALL
					   (SELECT
					   		cs.sbaid
					   	FROM
						 	cte.conveniosubacao cs
						INNER JOIN cte.categoriasubacao cts ON cts.sbaid = cs.sbaid AND casexibir = TRUE
					   	WHERE
					   		cs.sbaid  = ".$subacao['sbaid']." AND csustatus = 'A')";
			
			$dados = $db->carregar($sqlQtd);			   		
			
			$qtd = count($dados);
			
			if( $qtd > 1 ){
				$concluido = $concluido / $qtd;
			}
				
			if($concluido  == 49 || $concluido  == 50  || $concluido  == 51 ){
				$cor 	= "#FFFFFF";
				$cor1 	= "black";
			}else if($concluido  < 48){
				$cor 	= "#cococo";
				$cor1 	= "black";
			}else if ($concluido  > 51 ){
				$cor 	= "#FFFFFF";
				$cor1 	= "#FFFFFF";
			}
						
			if($concluido > 100){
				$corBarra = 100;
			}else{
				$corBarra = $concluido;
			}
			
			$barra 	=  '';
			$barra .= 	  '<div class="fundo" >'.
								'<div class="barra1" >'.
									'<div style=" text-align:center; position: absolute; width: 40px; height: 8px; padding: 0 0 0 0;  margin-bottom: 1px; " >'.
										'<div style=" color:'.$cor.'; font-size: 8px; max-height: 8px;  ">'.number_format($concluido, 2, ',', '.').'<span style="color:'.$cor1.'" >%</span></div>'.
									'</div>'.
									'<img class="imgBarra" style="width: '.$corBarra.'%;" src="../../imagens/cor1.gif"/>'.
								'</div>'.
							'</div>'.
						'';
			unset($concluido);
			$subacao['sbadsc'] = delimitador($subacao['sbadsc']);	
			
			$pergunta = '';
			
			if( $subacao['possuiduv'] > 0 ){
				
				$sql = "SELECT DISTINCT
							mdusituacao
						FROM 
							cte.monitoramentoduvidas md
						INNER JOIN cte.monitoramentosubacoes ms ON ms.mosid = md.mosid AND mosstatus = 'A'
						INNER JOIN cte.categoriasubacao     cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
						LEFT JOIN cte.conveniosubacao        cs ON cs.sbaid = ms.sbaid AND csustatus = 'A'
						LEFT JOIN cte.conveniomonitoramento  cm ON cm.cmoid = cs.cmoid	
						WHERE
							ms.sbaid = {$subacao['sbaid']}
							AND ms.mosano = {$_REQUEST['ano']}
							AND cm.cmoid {$convParecer}";
				$dados = $db->carregarColuna($sql);
				if( in_array('D',$dados) ){
					$pergunta .= '<img src="/imagens/email.gif" align="middle"/>&nbsp;';
				}
				if( in_array('A',$dados) ){
					$pergunta .= '<img src="/imagens/email_lido.gif" align="middle"/>&nbsp;';
				}
			}
			
			//$subacao['ssudescricao'] = $subacao['ssudescricao'] ? $subacao['ssudescricao'] : "";
			//if( brp_validarSubAcaoFaseAnalise( $subacao ) ){	
			//	$icone = '<img src="../imagens/check_p.gif" title="'. $subacao['ssudescricao'] .'"/>'.$pergunta.$barra.'&nbsp;';
			//}else {
			//	$icone = '<img src="../imagens/atencao.png" title="'. $subacao['ssudescricao'] .'"/>'.$pergunta.$barra.'&nbsp;';
			//}
			$icone = $pergunta.$barra.'&nbsp;';
			
			$sql = "SELECT DISTINCT
						mpasituacao
					FROM
						cte.monitoramentosubacoes ms
					INNER JOIN cte.categoriasubacao    cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
					LEFT JOIN cte.conveniosubacao       cs ON cs.sbaid = ms.sbaid AND csustatus = 'A'
					LEFT JOIN cte.conveniomonitoramento cm ON cm.cmoid = cs.cmoid	
					LEFT JOIN cte.monitoramentoparecer  mp ON mp.mosid = ms.mosid
					WHERE
						ms.sbaid = {$subacao['sbaid']}
						AND ms.mosano = {$_REQUEST['ano']}
						AND mosstatus = 'A'
						AND cm.cmoid {$convParecer}";
			
			$coluna = $db->carregarColuna($sql);
			unset($sql);
			
			if( in_array('P',$coluna) ){
				$ic = '<img src="../imagens/atencao.png" title="Parecer."/>&nbsp;';
			}
			if( in_array('A',$coluna) ){
				$ic .= '<img src="../imagens/check.jpg"   title="Atendido."/>&nbsp;';
			}
			$icone = $ic.$pergunta.$barra.'&nbsp;';
			unset($ic);
//			if( $subacao['mosaprovado'] == 'f' ){
//				$icone = '<img src="../imagens/atencao.png" title="Parecer."/>'.$pergunta.$barra.'&nbsp;';
//			}
			
			$prefixo = '';
			$iconeConvenio = "";
			
			$ordem = $subacao['sbaordem'] ? $subacao['sbaordem']."&nbsp;" : "";
			
			//Trata texto se convenioado ou nao e se filtro de convenio excolhido
			if( $subacao['conveniado'] > 0 ){
				if( $_REQUEST['cmoid'] != '' ){
					
					$sql="SELECT
							csuid
						  FROM
						  	cte.conveniosubacao
						  WHERE
						  	sbaid = {$subacao['sbaid'] } AND
						  	cmoid = {$_REQUEST['cmoid']} AND 
						  	csustatus = 'A'";
					
					$csuid = $db->pegaUm($sql);
					
					if( $subacao['mosalterado'] == 't' ){
						$texto = $icone . ( $barra ? $pimp : "" ) . $ordem . 
								 '<a href="javascript:monitorarSubacao('. $subacao['sbaid'] .','.$csuid.','.$_REQUEST['cmoid'].',\\\'t\\\');"><strong>'. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ",$subacao['dimcod'].".".$subacao['ardcod'].".".$subacao['indcod']." ".$subacao['sbadsc'] ).'</strong></a><a style=\"color:red;\"> ('.$subacao['convenio'].')</a>';
					}else{
						$texto = $icone . ( $barra ? $pimp : "" ) . $ordem . 
								 '<a href="javascript:monitorarSubacao('. $subacao['sbaid'] .','.$csuid.','.$_REQUEST['cmoid'].',\\\'f\\\');">'. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ",$subacao['dimcod'].".".$subacao['ardcod'].".".$subacao['indcod']." ".$subacao['sbadsc'] ).'</a><a style=\"color:red;\"> ('.$subacao['convenio'].')</a>';
					}
					
				}else{
					$sql = "SELECT
								cm.cmodsc || '/' || cm.cmoano as convenio
							FROM
								cte.conveniosubacao cs
							INNER JOIN	
								cte.conveniomonitoramento cm ON cm.cmoid = cs.cmoid
							WHERE
								cs.sbaid = {$subacao['sbaid']} AND csustatus = 'A'";
					
					$arConvenios = $db->carregar($sql);
					
					$convenios = '';
					
					foreach ( $arConvenios as $dado ){
						$convenios .= $convenios == '' ? $dado['convenio'] : ','.$dado['convenio'] ;
					}
					
					$zero = '\\\'nulo\\\'';
					$texto = $icone . ( $barra ? $pimp : "" ) . $ordem . 
							 ''. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ",$subacao['dimcod'].".".$subacao['ardcod'].".".$subacao['indcod']." ".$subacao['sbadsc'] ).'<a style=\"color:red;\"> - Filtrar por convênio. ('.$convenios.')</a>';
				}
			}else{
				$zero = '\\\'nulo\\\'';
				if( $subacao['mosalterado'] == 't' ){
					$texto = $icone . ( $barra ? $pimp : "" ) . $ordem . 
							 '<a href="javascript:monitorarSubacao('. $subacao['sbaid'] .','. $zero .','. $zero .',\\\'t\\\');"><strong>'. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ",$subacao['dimcod'].".".$subacao['ardcod'].".".$subacao['indcod']." ".$subacao['sbadsc'] ).'</strong></a>';
				}else{
					$texto = $icone . ( $barra ? $pimp : "" ) . $ordem . 
							 '<a href="javascript:monitorarSubacao('. $subacao['sbaid'] .','. $zero .','. $zero .',\\\'f\\\');">'. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ",$subacao['dimcod'].".".$subacao['ardcod'].".".$subacao['indcod']." ".$subacao['sbadsc'] ).'</a>';
				}
				
			}
			
			echo "arvore.add( 'sa_".$subacao['sbaid']."', 1, '".$prefixo.$texto."', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif' );\n";
			
			unset($concluido);
		}
		
		//============= Assistência Técnica =============
		$concluido = 0;
		unset($concluido);
		$sqlVlr = " SELECT
						ms.sbaid,
						ms.csuid,
						SUM(facpeso) as peso
					FROM cte.fasescategoria fc
					INNER JOIN cte.monitoramentosubacoes       ms ON ms.facid = fc.facid AND mosstatus = 'A'
					INNER JOIN cte.categoriasubacao           cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
					INNER JOIN cte.subacaoindicador            sa ON sa.sbaid = ms.sbaid
					INNER JOIN cte.acaoindicador               ai ON ai.aciid  = sa.aciid 
					INNER JOIN cte.pontuacao                    p ON p.ptoid   = ai.ptoid AND p.ptostatus = 'A'
					INNER JOIN cte.instrumentounidade         inu ON inu.inuid = p.inuid
					WHERE 
						ms.mosfinalizado = 't' AND
						sa.frmid = 15 AND 
						inu.estuf = '{$_REQUEST['estuf']}' AND 
						ms.mosano = {$_REQUEST['ano']}
					GROUP BY
						ms.sbaid,ms.csuid";
		
		$dados = $db->carregar($sqlVlr);
		
		$sbaid = 0;
		$csuid = 0;
		unset($vlrTotal);
		if(is_array($dados)){
			foreach($dados as $valor ){
				if( !($sbaid == $valor['sbaid']) ){
					$sbaid    = $valor['sbaid'];
					$scuid    = $valor['csuid'];
					$vlrTotal = $vlrTotal+$valor['peso'];
				}
			}
		}
		
		$sqlQtd = "(SELECT DISTINCT
						sa.sbaid 
					FROM cte.subacaoindicador sa
					LEFT  JOIN cte.monitoramentosubacoes       ms ON sa.sbaid = ms.sbaid AND mosstatus = 'A'
					INNER JOIN cte.categoriasubacao           cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
					LEFT  JOIN cte.conveniosubacao             cs ON cs.sbaid  = sa.sbaid AND csustatus = 'A'
					INNER JOIN cte.acaoindicador               ai ON ai.aciid  = sa.aciid 
					INNER JOIN cte.pontuacao                    p ON p.ptoid   = ai.ptoid AND p.ptostatus = 'A'
					INNER JOIN cte.instrumentounidade         inu ON inu.inuid = p.inuid
					--INNER JOIN cte.projetosapesubacao         pss ON pss.sbaid = sa.sbaid AND pss.pssano = {$_REQUEST['ano']}
					INNER JOIN cte.subacaoparecertecnico      spt ON spt.sbaid = sa.sbaid AND cast(spt.sptano AS varchar) = '{$_REQUEST['ano']}'
					WHERE 
						sa.frmid = 15 AND 
						inu.estuf = '{$_REQUEST['estuf']}'  AND ms.csuid is null AND cs.sbaid is null)
					UNION ALL
				   (SELECT
				   		sa.sbaid
					FROM cte.subacaoindicador sa
					INNER JOIN cte.acaoindicador               ai ON ai.aciid  = sa.aciid 
					INNER JOIN cte.pontuacao                    p ON p.ptoid   = ai.ptoid AND p.ptostatus = 'A'
					INNER JOIN cte.instrumentounidade         inu ON inu.inuid = p.inuid
					--INNER JOIN cte.projetosapesubacao         pss ON pss.sbaid = sa.sbaid AND pss.pssano = {$_REQUEST['ano']}
					INNER JOIN cte.subacaoparecertecnico      spt ON spt.sbaid = sa.sbaid AND cast(spt.sptano AS varchar) = '{$_REQUEST['ano']}'
					INNER JOIN cte.conveniosubacao             cs ON cs.sbaid  = sa.sbaid AND csustatus = 'A'
					WHERE 
						sa.frmid = 15 AND 
						inu.estuf = '{$_REQUEST['estuf']}')";
		
		$dados = $db->carregar($sqlQtd);
		
		$qtd = count($dados);
		
		if( $qtd > 1 ){
			$concluido = $vlrTotal/$qtd;
		}
						
		if($concluido  == 49 || $concluido  == 50  || $concluido  == 51 ){
			$cor 	= "#FFFFFF";
			$cor1 	= "black";
		}else if($concluido  < 48){
			$cor 	= "#cococo";
			$cor1 	= "black";
		}else if ($concluido  > 51 ){
			$cor 	= "#FFFFFF";
			$cor1 	= "#FFFFFF";
		}
					
		if($concluido > 100){
			$corBarra = 100;
		}else{
			$corBarra = $concluido;
		}
		
		$barra 	=  '';
		$barra .= 	  '<div class="fundo" >'.
							'<div class="barra1" >'.
								'<div style=" text-align:center; position: absolute; width: 40px; height: 8px; padding: 0 0 0 0;  margin-bottom: 1px; " >'.
									'<div style=" color:'.$cor.'; font-size: 8px; max-height: 8px;  ">'.number_format($concluido, 2, ',', '.').'<span style="color:'.$cor1.'" >%</span></div>'.
								'</div>'.
								'<img class="imgBarra" style="width: '.$corBarra.'%;" src="../../imagens/cor1.gif"/>'.
							'</div>'.
						'</div>'.
					'';
		unset($concluido);
		echo "arvore.add( 2, 0, '<b>Assistência Técnica ".$barra.$pimp."</b>', '', '', '', '../includes/dtree/img/globe.gif', '../includes/dtree/img/globe.gif' );\n";
		
		$barra 	=  '';
		
		foreach( $subacoes_at as $subacao ){ 
			
			$barra = '';
			
			unset($concluido);
			
			$sql = "SELECT 
						fc.facpeso, 
						ms.mosfinalizado
					FROM 
						cte.monitoramentosubacoes ms
					INNER JOIN cte.fasescategoria    fc ON fc.facid = ms.facid
					INNER JOIN cte.categoriasubacao cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
					WHERE 
						ms.mosano = ".$_REQUEST['ano']." AND 
						ms.sbaid  = ".$subacao['sbaid']." AND 
						ms.mosstatus = 'A'";
			
			$facpeso = $db->carregar($sql);
			
			if($facpeso){
				
				$concluido = 0;
				$r = 0;
				foreach( $facpeso as $peso ){
					if ( $peso['mosfinalizado'] == 't'){
						$concluido = $concluido+$peso['facpeso'];
					}
					$r++;
				}
			}else{
				$concluido = 0;
			}
			
			$sqlQtd = "(SELECT DISTINCT
							ms.sbaid
						 FROM
						 	cte.monitoramentosubacoes ms
						 INNER JOIN cte.categoriasubacao cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
						 WHERE
						 	ms.sbaid  = ".$subacao['sbaid']."  AND csuid is null AND mosstatus = 'A') 
						 UNION ALL
					   (SELECT
					   		cs.sbaid
					   	FROM
						 	cte.conveniosubacao cs
						INNER JOIN cte.categoriasubacao cts ON cts.sbaid = cs.sbaid AND casexibir = TRUE
					   	WHERE
					   		cs.sbaid  = ".$subacao['sbaid']." AND csustatus = 'A')";
			
			$dados = $db->carregar($sqlQtd);			   		
			
			$qtd = count($dados);
			
			if( $qtd > 1 ){
				$concluido = $concluido / $qtd;
			}
				
			if($concluido  == 49 || $concluido  == 50  || $concluido  == 51 ){
				$cor 	= "#FFFFFF";
				$cor1 	= "black";
			}else if($concluido  < 48){
				$cor 	= "#cococo";
				$cor1 	= "black";
			}else if ($concluido  > 51 ){
				$cor 	= "#FFFFFF";
				$cor1 	= "#FFFFFF";
			}
						
			if($concluido > 100){
				$corBarra = 100;
			}else{
				$corBarra = $concluido;
			}
			
			$barra 	=  '';
			$barra .= 	  '<div class="fundo" >'.
								'<div class="barra1" >'.
									'<div style=" text-align:center; position: absolute; width: 40px; height: 8px; padding: 0 0 0 0;  margin-bottom: 1px; " >'.
										'<div style=" color:'.$cor.'; font-size: 8px; max-height: 8px;  ">'.number_format($concluido, 2, ',', '.').'<span style="color:'.$cor1.'" >%</span></div>'.
									'</div>'.
									'<img class="imgBarra" style="width: '.$corBarra.'%;" src="../../imagens/cor1.gif"/>'.
								'</div>'.
							'</div>'.
						'';
			unset($concluido);
			$subacao['sbadsc'] = delimitador($subacao['sbadsc']);	
			
			$pergunta = '';
			
			if( $subacao['possuiduv'] > 0 ){
				
				$sql = "SELECT DISTINCT
							mdusituacao
						FROM 
							cte.monitoramentoduvidas md
						INNER JOIN cte.monitoramentosubacoes ms ON ms.mosid = md.mosid AND mosstatus = 'A'
						INNER JOIN cte.categoriasubacao     cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
						LEFT JOIN cte.conveniosubacao        cs ON cs.sbaid = ms.sbaid AND csustatus = 'A'
						LEFT JOIN cte.conveniomonitoramento  cm ON cm.cmoid = cs.cmoid	
						WHERE
							ms.sbaid = {$subacao['sbaid']}
							AND ms.mosano = {$_REQUEST['ano']}
							AND cm.cmoid {$convParecer}";
				$dados = $db->carregarColuna($sql);
				if( in_array('D',$dados) ){
					$pergunta .= '<img src="/imagens/email.gif" align="middle"/>&nbsp;';
				}
				if( in_array('A',$dados) ){
					$pergunta .= '<img src="/imagens/email_lido.gif" align="middle"/>&nbsp;';
				}
			}
			
			//$subacao['ssudescricao'] = $subacao['ssudescricao'] ? $subacao['ssudescricao'] : "";
			//if( brp_validarSubAcaoFaseAnalise( $subacao ) ){	
			//	$icone = '<img src="../imagens/check_p.gif" title="'. $subacao['ssudescricao'] .'"/>'.$pergunta.$barra.'&nbsp;';
			//}else {
			//	$icone = '<img src="../imagens/atencao.png" title="'. $subacao['ssudescricao'] .'"/>'.$pergunta.$barra.'&nbsp;';
			//}
			$icone = $pergunta.$barra.'&nbsp;';
			
			$sql = "SELECT DISTINCT
						mpasituacao
					FROM
						cte.monitoramentosubacoes ms
					INNER JOIN cte.categoriasubacao     cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
					LEFT JOIN cte.conveniosubacao        cs ON cs.sbaid = ms.sbaid AND csustatus = 'A'
					LEFT JOIN cte.conveniomonitoramento  cm ON cm.cmoid = cs.cmoid	
					LEFT JOIN cte.monitoramentoparecer   mp ON mp.mosid = ms.mosid
					WHERE
						ms.sbaid = {$subacao['sbaid']}
						AND ms.mosano = {$_REQUEST['ano']}
						AND cm.cmoid {$convParecer}
						AND mosstatus = 'A'";
			
			$coluna = $db->carregarColuna($sql);
			unset($sql);
			
			if( in_array('P',$coluna) ){
				$ic = '<img src="../imagens/atencao.png" title="Parecer."/>&nbsp;';
			}
			if( in_array('A',$coluna) ){
				$ic .= '<img src="../imagens/check.jpg"   title="Atendido."/>&nbsp;';
			}
			$icone = $ic.$pergunta.$barra.'&nbsp;';
			unset($ic);
			
//			if( $subacao['mosaprovado'] == 'f' ){
//				$icone = '<img src="../imagens/atencao.png" title="Parecer"/>'.$pergunta.$barra.'&nbsp;';
//			}
	
			$prefixo = '';
			$iconeConvenio = "";
	
			$ordem = $subacao['sbaordem'] ? $subacao['sbaordem']."&nbsp;" : "";
	
			//Trata texto se convenioado ou nao e se filtro de convenio excolhido
			if( $subacao['conveniado'] > 0 ){
				if( $_REQUEST['cmoid'] != '' ){
					
					$sql="SELECT
							csuid
						  FROM
						  	cte.conveniosubacao
						  WHERE
						  	sbaid = {$subacao['sbaid'] } AND
						  	cmoid = {$_REQUEST['cmoid']} AND
						  	csustatus = 'A'";
					
					$csuid = $db->pegaUm($sql);
					
					if( $subacao['mosalterado'] == 't' ){
						$texto = $icone . ( $barra ? $pimp : "" ) . $ordem . 
								 '<a href="javascript:monitorarSubacao('. $subacao['sbaid'] .','.$csuid.','.$_REQUEST['cmoid'].',\\\'t\\\');"><strong>'. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ",$subacao['dimcod'].".".$subacao['ardcod'].".".$subacao['indcod']." ".$subacao['sbadsc'] ).'</strong></a><a style=\"color:red;\"> ('.$subacao['convenio'].')</a>';
					}else{
						$texto = $icone . ( $barra ? $pimp : "" ) . $ordem . 
								 '<a href="javascript:monitorarSubacao('. $subacao['sbaid'] .','.$csuid.','.$_REQUEST['cmoid'].',\\\'f\\\');">'. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ",$subacao['dimcod'].".".$subacao['ardcod'].".".$subacao['indcod']." ".$subacao['sbadsc'] ).'</a><a style=\"color:red;\"> ('.$subacao['convenio'].')</a>';
					}
					
				}else{
					$sql = "SELECT
								cm.cmodsc || '/' || cm.cmoano as convenio
							FROM
								cte.conveniosubacao cs
							INNER JOIN	
								cte.conveniomonitoramento cm ON cm.cmoid = cs.cmoid
							WHERE
								cs.sbaid = {$subacao['sbaid']} AND csustatus = 'A'";
					
					$arConvenios = $db->carregar($sql);
					
					$convenios = '';
					
					foreach ( $arConvenios as $dado ){
						$convenios .= $convenios == '' ? $dado['convenio'] : ','.$dado['convenio'] ;
					}
					
					$zero = '\\\'nulo\\\'';
					$texto = $icone . ( $barra ? $pimp : "" ) . $ordem . 
							 ''. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ",$subacao['dimcod'].".".$subacao['ardcod'].".".$subacao['indcod']." ".$subacao['sbadsc'] ).'<a style=\"color:red;\"> - Filtrar por convênio. ('.$convenios.')</a>';
				}
			}else{
				$zero = '\\\'nulo\\\'';
				
				if( $subacao['mosalterado'] == 't' ){
					$texto = $icone . ( $barra ? $pimp : "" ) . $ordem . 
							 '<a href="javascript:monitorarSubacao('. $subacao['sbaid'] .','. $zero .','. $zero .',\\\'t\\\');"><strong>'. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ",$subacao['dimcod'].".".$subacao['ardcod'].".".$subacao['indcod']." ".$subacao['sbadsc'] ).'</strong></a>';
				}else{
					$texto = $icone . ( $barra ? $pimp : "" ) . $ordem . 
							 '<a href="javascript:monitorarSubacao('. $subacao['sbaid'] .','. $zero .','. $zero .',\\\'f\\\');">'. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ",$subacao['dimcod'].".".$subacao['ardcod'].".".$subacao['indcod']." ".$subacao['sbadsc'] ).'</a>';
				}				
			}
			
			echo "arvore.add( 'sa_".$subacao['sbaid']."', 2, '".$prefixo.$texto."', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif' );\n";
		
			unset($concluido);
		}
		
		//====== Execução Direta do Estado
		
		$concluido = 0;

		$sqlVlr = " SELECT
						ms.sbaid,
						ms.csuid,
						SUM(facpeso) as peso
					FROM cte.fasescategoria fc
					INNER JOIN cte.monitoramentosubacoes       ms ON ms.facid = fc.facid AND mosstatus = 'A'
					INNER JOIN cte.categoriasubacao     	  cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
					INNER JOIN cte.subacaoindicador            sa ON sa.sbaid = ms.sbaid
					INNER JOIN cte.acaoindicador               ai ON ai.aciid  = sa.aciid 
					INNER JOIN cte.pontuacao                    p ON p.ptoid   = ai.ptoid AND p.ptostatus = 'A'
					INNER JOIN cte.instrumentounidade         inu ON inu.inuid = p.inuid
					WHERE 
						ms.mosfinalizado = 't' AND
						sa.frmid = 13 AND 
						inu.estuf = '{$_REQUEST['estuf']}' AND 
						ms.mosano = {$_REQUEST['ano']}
					GROUP BY
						ms.sbaid,ms.csuid";
		
		$dados = $db->carregar($sqlVlr);
		
		$sbaid = 0;
		$csuid = 0;
		unset($vlrTotal);
		if(is_array($dados)){
			foreach($dados as $valor ){
				if( !($sbaid == $valor['sbaid']) ){
					$sbaid    = $valor['sbaid'];
					$scuid    = $valor['csuid'];
					$vlrTotal = $vlrTotal+$valor['peso'];
				}
			}
		}
		
		$sqlQtd = "(SELECT DISTINCT
						sa.sbaid 
					FROM cte.subacaoindicador sa
					LEFT  JOIN cte.monitoramentosubacoes       ms ON sa.sbaid = ms.sbaid AND mosstatus = 'A'
					INNER JOIN cte.categoriasubacao     	  cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
					LEFT  JOIN cte.conveniosubacao             cs ON cs.sbaid  = sa.sbaid AND csustatus = 'A'
					INNER JOIN cte.acaoindicador               ai ON ai.aciid  = sa.aciid 
					INNER JOIN cte.pontuacao                    p ON p.ptoid   = ai.ptoid AND p.ptostatus = 'A'
					INNER JOIN cte.instrumentounidade         inu ON inu.inuid = p.inuid
					--INNER JOIN cte.projetosapesubacao         pss ON pss.sbaid = sa.sbaid AND pss.pssano = {$_REQUEST['ano']}
					INNER JOIN cte.subacaoparecertecnico      spt ON spt.sbaid = sa.sbaid AND cast(spt.sptano AS varchar) = '{$_REQUEST['ano']}'
					WHERE 
						sa.frmid = 13 AND 
						inu.estuf = '{$_REQUEST['estuf']}'  AND ms.csuid is null AND cs.sbaid is null)
					UNION ALL
				   (SELECT DISTINCT
				   		sa.sbaid
					FROM cte.subacaoindicador sa
					INNER JOIN cte.acaoindicador               ai ON ai.aciid  = sa.aciid 
					INNER JOIN cte.pontuacao                    p ON p.ptoid   = ai.ptoid AND p.ptostatus = 'A'
					INNER JOIN cte.instrumentounidade         inu ON inu.inuid = p.inuid
					--INNER JOIN cte.projetosapesubacao         pss ON pss.sbaid = sa.sbaid AND pss.pssano = {$_REQUEST['ano']}
					INNER JOIN cte.subacaoparecertecnico      spt ON spt.sbaid = sa.sbaid AND cast(spt.sptano AS varchar) = '{$_REQUEST['ano']}' 
					INNER JOIN cte.conveniosubacao             cs ON cs.sbaid  = sa.sbaid AND csustatus = 'A'
					WHERE 
						sa.frmid = 13 AND 
						inu.estuf = '{$_REQUEST['estuf']}')";
		
		$dados = $db->carregar($sqlQtd);
		
		$qtd = count($dados);
		
		if( $qtd > 0 ){
			$concluido = $vlrTotal/$qtd;
		}
						
		if($concluido  == 49 || $concluido  == 50  || $concluido  == 51 ){
			$cor 	= "#FFFFFF";
			$cor1 	= "black";
		}else if($concluido  < 48){
			$cor 	= "#cococo";
			$cor1 	= "black";
		}else if ($concluido  > 51 ){
			$cor 	= "#FFFFFF";
			$cor1 	= "#FFFFFF";
		}
					
		if($concluido > 100){
			$corBarra = 100;
		}else{
			$corBarra = $concluido;
		}
		
		$barra 	=  '';
		$barra .= 	  '<div class="fundo" >'.
							'<div class="barra1" >'.
								'<div style=" text-align:center; position: absolute; width: 40px; height: 8px; padding: 0 0 0 0;  margin-bottom: 1px; " >'.
									'<div style=" color:'.$cor.'; font-size: 8px; max-height: 8px;  ">'.number_format($concluido, 2, ',', '.').'<span style="color:'.$cor1.'" >%</span></div>'.
								'</div>'.
								'<img class="imgBarra" style="width: '.$corBarra.'%;" src="../../imagens/cor1.gif"/>'.
							'</div>'.
						'</div>'.
					'';
		
		echo "arvore.add( 3, 0, '<b>Execução Direta do Estado ".$barra.$pimp."</b>', '', '', '', '../includes/dtree/img/globe.gif', '../includes/dtree/img/globe.gif' );\n";
		
		$barra 	=  '';
		
		foreach( $subacoes_ede as $subacao ){ 
			
			$barra = '';
			
			$sql = "SELECT 
						fc.facpeso, 
						ms.mosfinalizado
					FROM 
						cte.monitoramentosubacoes ms
					INNER JOIN cte.categoriasubacao cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
					INNER JOIN cte.fasescategoria    fc ON fc.facid = ms.facid
					WHERE 
						ms.mosano = ".$_REQUEST['ano']." AND 
						ms.sbaid  = ".$subacao['sbaid']."
						AND mosstatus = 'A'";
			
			$facpeso = $db->carregar($sql);
			
			if($facpeso){
				
				$concluido = 0;
				$r = 0;
				foreach( $facpeso as $peso ){
					if ( $peso['mosfinalizado'] == 't'){
						$concluido = $concluido+$peso['facpeso'];
					}
					$r++;
				}
			}else{
				$concluido = 0;
			}
			
			$sqlQtd = "(SELECT DISTINCT
							ms.sbaid
						 FROM
						 	cte.monitoramentosubacoes ms
						 INNER JOIN cte.categoriasubacao cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
						 WHERE
						 	ms.sbaid  = ".$subacao['sbaid']."  AND csuid is null AND mosstatus = 'A') 
						 UNION ALL
					   (SELECT
					   		cs.sbaid
					   	FROM
						 	cte.conveniosubacao cs
						INNER JOIN cte.categoriasubacao cts ON cts.sbaid = cs.sbaid AND casexibir = TRUE
					   	WHERE
					   		cs.sbaid  = ".$subacao['sbaid']." AND csustatus = 'A')";
			
			$dados = $db->carregar($sqlQtd);			   		
			
			$qtd = count($dados);
			
			if( $qtd > 1 ){
				$concluido = $concluido / $qtd;
			}
			
			if($concluido  == 49 || $concluido  == 50  || $concluido  == 51 ){
				$cor 	= "#FFFFFF";
				$cor1 	= "black";
			}else if($concluido  < 48){
				$cor 	= "#cococo";
				$cor1 	= "black";
			}else if ($concluido  > 51 ){
				$cor 	= "#FFFFFF";
				$cor1 	= "#FFFFFF";
			}
						
			if($concluido > 100){
				$corBarra = 100;
			}else{
				$corBarra = $concluido;
			}
			
			$barra 	=  '';
			$barra .= 	  '<div class="fundo" >'.
								'<div class="barra1" >'.
									'<div style=" text-align:center; position: absolute; width: 40px; height: 8px; padding: 0 0 0 0;  margin-bottom: 1px; " >'.
										'<div style=" color:'.$cor.'; font-size: 8px; max-height: 8px;  ">'.number_format($concluido, 2, ',', '.').'<span style="color:'.$cor1.'" >%</span></div>'.
									'</div>'.
									'<img class="imgBarra" style="width: '.$corBarra.'%;" src="../../imagens/cor1.gif"/>'.
								'</div>'.
							'</div>'.
						'';
			
			$subacao['sbadsc'] = delimitador($subacao['sbadsc']);	
			
			$pergunta = '';
			
			if( $subacao['possuiduv'] > 0 ){
				
				$sql = "SELECT DISTINCT
							mdusituacao
						FROM 
							cte.monitoramentoduvidas md
						INNER JOIN cte.monitoramentosubacoes ms ON ms.mosid = md.mosid AND mosstatus = 'A'
						INNER JOIN cte.categoriasubacao     cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
						LEFT JOIN  cte.conveniosubacao       cs ON cs.sbaid = ms.sbaid AND csustatus = 'A'
						LEFT JOIN  cte.conveniomonitoramento cm ON cm.cmoid = cs.cmoid	
						WHERE
							ms.sbaid = {$subacao['sbaid']}
							AND ms.mosano = {$_REQUEST['ano']}
							AND cm.cmoid {$convParecer}";
				$dados = $db->carregarColuna($sql);
				if( in_array('D',$dados) ){
					$pergunta .= '<img src="/imagens/email.gif" align="middle"/>&nbsp;';
				}
				if( in_array('A',$dados) ){
					$pergunta .= '<img src="/imagens/email_lido.gif" align="middle"/>&nbsp;';
				}
			}
			
			//$subacao['ssudescricao'] = $subacao['ssudescricao'] ? $subacao['ssudescricao'] : "";
			//if( brp_validarSubAcaoFaseAnalise( $subacao ) ){
			//	$icone = '<img src="../imagens/check_p.gif" title="'. $subacao['ssudescricao'] .'"/>'.$pergunta.$barra.'&nbsp;';
			//}else {
			//	$icone = '<img src="../imagens/atencao.png" title="'. $subacao['ssudescricao'] .'"/>'.$pergunta.$barra.'&nbsp;';
			//}
			$icone = $pergunta.$barra.'&nbsp;';
			
			$sql = "SELECT DISTINCT
						mpasituacao
					FROM
						cte.monitoramentosubacoes ms
					INNER JOIN cte.categoriasubacao     cts ON cts.sbaid = ms.sbaid AND casexibir = TRUE
					LEFT  JOIN cte.conveniosubacao    	 cs ON cs.sbaid = ms.sbaid AND csustatus = 'A'
					LEFT  JOIN cte.conveniomonitoramento cm ON cm.cmoid = cs.cmoid	
					LEFT  JOIN cte.monitoramentoparecer  mp ON mp.mosid = ms.mosid
					WHERE
						ms.sbaid = {$subacao['sbaid']}
						AND ms.mosano = {$_REQUEST['ano']}
						AND cm.cmoid {$convParecer}
						AND mosstatus = 'A'";
			
			$coluna = $db->carregarColuna($sql);
			unset($sql);
			
			if( in_array('P',$coluna) ){
				$ic = '<img src="../imagens/atencao.png" title="Parecer."/>&nbsp;';
			}
			if( in_array('A',$coluna) ){
				$ic .= '<img src="../imagens/check.jpg"   title="Atendido."/>&nbsp;';
			}
			$icone = $ic.$pergunta.$barra.'&nbsp;';
			unset($ic);
			
//			if( $subacao['mosaprovado'] == 'f' ){
//				$icone = '<img src="../imagens/atencao.png" title="Parecer"/>'.$pergunta.$barra.'&nbsp;';
//			}
	
			$prefixo = '';
			$iconeConvenio = "";
	
			$ordem = $subacao['sbaordem'] ? $subacao['sbaordem']."&nbsp;" : "";
			
			//Trata texto se convenioado ou nao e se filtro de convenio excolhido
			if( $subacao['conveniado'] > 0 ){
				if( $_REQUEST['cmoid'] != '' ){
					
					$sql="SELECT
							csuid
						  FROM
						  	cte.conveniosubacao
						  WHERE
						  	sbaid = {$subacao['sbaid'] } AND
						  	cmoid = {$_REQUEST['cmoid']} AND 
						  	csustatus = 'A'";
					
					$csuid = $db->pegaUm($sql);
					
					if( $subacao['mosalterado'] == 't' ){
						$texto = $icone . ( $barra ? $pimp : "" ) . $ordem . 
								 '<a href="javascript:monitorarSubacao('. $subacao['sbaid'] .','.$csuid.','.$_REQUEST['cmoid'].',\\\'t\\\');"><strong>'. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ",$subacao['dimcod'].".".$subacao['ardcod'].".".$subacao['indcod']." ".$subacao['sbadsc'] ).'</strong></a><a style=\"color:red;\"> ('.$subacao['convenio'].')</a>';
					}else{
						$texto = $icone . ( $barra ? $pimp : "" ) . $ordem . 
								 '<a href="javascript:monitorarSubacao('. $subacao['sbaid'] .','.$csuid.','.$_REQUEST['cmoid'].',\\\'f\\\');">'. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ",$subacao['dimcod'].".".$subacao['ardcod'].".".$subacao['indcod']." ".$subacao['sbadsc'] ).'</a><a style=\"color:red;\"> ('.$subacao['convenio'].')</a>';
					}
					
				}else{
					$sql = "SELECT
								cm.cmodsc || '/' || cm.cmoano as convenio
							FROM
								cte.conveniosubacao cs
							INNER JOIN	
								cte.conveniomonitoramento cm ON cm.cmoid = cs.cmoid
							WHERE
								cs.sbaid = {$subacao['sbaid']} AND csustatus = 'A'";
					
					$arConvenios = $db->carregar($sql);
					
					$convenios = '';
					
					foreach ( $arConvenios as $dado ){
						$convenios .= $convenios == '' ? $dado['convenio'] : ','.$dado['convenio'] ;
					}
					
					$zero = '\\\'nulo\\\'';
					$texto = $icone . ( $barra ? $pimp : "" ) . $ordem . 
							 ''. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ",$subacao['dimcod'].".".$subacao['ardcod'].".".$subacao['indcod']." ".$subacao['sbadsc'] ).'<a style=\"color:red;\"> - Filtrar por convênio. ('.$convenios.')</a>';
				}
			}else{
				$zero = '\\\'nulo\\\'';
				
				if( $subacao['mosalterado'] == 't' ){
					$texto = $icone . ( $barra ? $pimp : "" ) . $ordem . 
							 '<a href="javascript:monitorarSubacao('. $subacao['sbaid'] .','. $zero .','. $zero .',\\\'t\\\');"><strong>'. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ",$subacao['dimcod'].".".$subacao['ardcod'].".".$subacao['indcod']." ".$subacao['sbadsc'] ).'</strong></a>';
				}else{
					$texto = $icone . ( $barra ? $pimp : "" ) . $ordem . 
							 '<a href="javascript:monitorarSubacao('. $subacao['sbaid'] .','. $zero .','. $zero .',\\\'f\\\');">'. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ",$subacao['dimcod'].".".$subacao['ardcod'].".".$subacao['indcod']." ".$subacao['sbadsc'] ).'</a>';
				}
			}
			
			echo "arvore.add( 'sa_".$subacao['sbaid']."', 3, '".$prefixo.$texto."', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif' );\n";
		
			unset($concluido);
		}
	}
?>
	elemento = document.getElementById( '_arvore' );
	elemento.innerHTML = arvore;

	/*
	 * adaptação de largura e controle de overflow de conteúdo para o internet explorer
	 */

	var dav = navigator.appVersion;

	IE = document.all ? true : false;
	IE6 = dav.indexOf( "MSIE 6.0" ) >= 0;
	IE7 = dav.indexOf( "MSIE 7.0" ) >= 0;

	if ( IE ) {

		width = document.body.offsetWidth;
		height = document.body.offsetHeight;
		document.getElementById( 'bloco' ).style.width = ( width * 0.80 ) + 'px';
	}


<? 
} 
?>
	var superuser = <?=cte_possuiPerfil(array(CTE_PERFIL_SUPER_USUARIO,CTE_PERFIL_ADMINISTRADOR )) ? 'true' : 'false'; ?>;

	function listarSubacoes(){
		
		var ano      = $('ano');
		var convenio = $('convenio');
		if( superuser == true ){
			estuf = $('estuf').value;
		}
		
		if( ano.value == ' --- '){
			alert('Escolha um ano.');
			ano.focus();
			return false;
		}
		
		window.location.href = 'brasilpro.php?modulo=principal/monitoramentoInterno/listaSubsFrm&acao=E&evento=selecionar&estuf='+estuf+'&ano='+ano.value+'&cmoid='+convenio.value;
	}
</script>





