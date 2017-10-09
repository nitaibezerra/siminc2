<?php

function verificaPendenciasDemandas(){

	global $db;

	return false;

	$sql = "SELECT
           		COUNT(dmd.dmdid) as qtd
	        FROM
	            par.demanda dmd
	            INNER JOIN workflow.documento 		doc ON dmd.docid = doc.docid AND doc.tpdid = 127
	            INNER JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid
	        WHERE
	            dmdstatus = 'A' AND dmd.dmdcpfsolicitante = '{$_SESSION['usucpf']}' AND esd.esdid IN (832, 834)";

	$qtd_demandas_atrazadas = $db->pegaUm( $sql );

	if( $qtd_demandas_atrazadas < 1 ){
		return false;
	}
?>
<style>
.qtd_pendencias {
	width: 50px; height: 50px; float: right; cursor: pointer; margin-right: 20px; border-radius: 25px 25px 25px 25px;
	background: -webkit-radial-gradient(center, ellipse cover, #e20000 17%,#ff0505 100%); box-shadow: 0 0 10px rgba(0, 0, 0, 0.5);
	text-align: center; font-size: 20px; font-family: arial; color: white; font-weight: bold; vertical-align: botton;
}
.qtd_pendencias:hover {
	background: -webkit-radial-gradient(center, ellipse cover, #ff0505 25%,#ee0505 60%);
}
.pendencias_detalhes{
	width: 150px; height: 80px; margin-top: -1px; float: right; margin-right: 10px; display: none;
	box-shadow: 0 0 10px rgba(0, 0, 0, 0.5); background-color: #ff0505;
	text-align: justify; font-size: 12px; font-family: arial; color: white; font-weight: bold; vertical-align: botton;
}
</style>
<script>
$1_11(document).ready(function(){

	jQuery('#demandas_pendencias').mouseover(function(){
		jQuery('#demandas_pendencias_detalhes').show();
	});
	jQuery('#demandas_pendencias').mouseout(function(){
		jQuery('#demandas_pendencias_detalhes').hide();
	});
	jQuery('#demandas_pendencias').click(function(){
		window.location.href = 'par.php?modulo=principal/demandas/minhasPendencias&acao=A';
	});
});
</script>
<div style="width: 200px; height: 200px; position:fixed; z-index: 50; right: 0; top: 135px;">
	<div id="demandas_pendencias" class="qtd_pendencias" >
		<div style="margin-top:13px;" id="demandas_qtd_pendencias" >
			<?=$qtd_demandas_atrazadas ?>
		</div>
	</div>
	<div id="demandas_pendencias_detalhes" class="pendencias_detalhes" >
		<div style="margin-top:10px; margin-left:10px; margin-right:10px;" id="demandas_pendencias_detalhes_texto" >
			Você possui <?=$qtd_demandas_atrazadas ?> demandas pendêntes de validação ou diligência.
		</div>
	</div>
</div>
<?
}

/* paginacaoAjax() - INSTRUÇÕES DE USO
 *
 * Essa função faz a paginação ajax e escreve apenas os números da paginação, podendo ser chamada dentro de tabelas, divs, desde que não esteja dentro de um formulário.
 *
 * Para ela funcionar ela deve ser alimentada com as seguintes informações, que devem segui o padrão explicado abaixo:
 *
 * - As requisições ajax devem, OBRIGATÓRIAMENTE usar o parametro 'req' do $_POST para chamar a função da requisição ajax;
 * - Devem ser informados id do container a receber o resultado do ajax, o numero máximo de paginas e a quantidade total de registros, da maneira abaixo:
 * 		° $_SESSION['paginacaoAjax'][$_POST['req']]['container']
 * 		° $_SESSION['paginacaoAjax'][$_POST['req']]['quantidade']
 * 		° $_SESSION['paginacaoAjax'][$_POST['req']]['por_pagina']
 *
 * - Ao clicar para paginar, o componente refará a requisição e enviará a mais um parametro 'pagina' no $_POST, o qual você poderá, junto com o $_SESSION['paginacaoAjax'][$_POST['req']]['por_pagina'],
 * 	usar para controlar o 'LIMIT' e o 'OFFSET', fazendo assim a paginação
 *
 * Existe um exemplo no Painel do PAR, nas listas de detalhamento
 * */
function paginacaoAjax(){

	$paginaAtual = $_POST['pagina'] > 0 ? $_POST['pagina'] : '1';
	unset($_POST['pagina']);

	$paginas 	= (int)($_SESSION['paginacaoAjax'][$_POST['req']]['quantidade']/$_SESSION['paginacaoAjax'][$_POST['req']]['por_pagina']);
	$resto 		= 		$_SESSION['paginacaoAjax'][$_POST['req']]['quantidade']%$_SESSION['paginacaoAjax'][$_POST['req']]['por_pagina'];

	if( $resto > 0 ){
		$paginas = $paginas+1;
	}

	$arrPaginas = Array();
	if( $paginas > 4 ){

		if( ($paginas - $paginaAtual) > 3 ){

			if( $paginaAtual < 4 ){
				$arrPaginas = Array(1,2,3,4,5);
			}
			if( $paginaAtual > 3 ){
				$arrPaginas = Array($paginaAtual-2,$paginaAtual-1,$paginaAtual,$paginaAtual+1,$paginaAtual+2);
			}
		}else{

			$arrPaginas = Array($paginas-4, $paginas-3, $paginas-2, $paginas-1, $paginas);
		}
	}
	if( $paginas == 4 ) $arrPaginas = Array(1,2,3,4);
	if( $paginas == 3 ) $arrPaginas = Array(1,2,3);
	if( $paginas == 2 ) $arrPaginas = Array(1,2);
	if( $paginas == 1 ) $arrPaginas = Array(1);

?>
	<script>
	$(document).ready(function(){

		$(document).off('click', '.pagina<?=$_POST['req'] ?>').on('click','.pagina<?=$_POST['req'] ?>',function(){

			var pagina = $(this).attr('pagina');
			$.ajax({
		   		type: "POST",
		   		url: window.location.href,
		   		data: "req=<?=$_POST['req'] ?>&pagina="+pagina+"&"+$('#formPaginacaoAjax<?=$_POST['req'] ?>').serialize(),
		   		async: false,
		   		success: function(msg){
		   			$( "#<?=$_SESSION['paginacaoAjax'][$_POST['req']]['container'] ?>" ).html(msg);
		   		}
			});
		});
	});
	</script>
	<form method="post" name="formPaginacaoAjax<?=$_POST['req'] ?>" id="formPaginacaoAjax<?=$_POST['req'] ?>" >
		<?php foreach( $_POST as $chave => $valor ) {?>
		<input type="hidden" name="<?=$chave ?>" value="<?=$valor ?>" />
		<?php }?>
	</form>
	<span style="background-color:#f5f5f5;BORDER-RIGHT: #a0a0a0 1px solid;text-decoration:none;">&nbsp;Paginas: &nbsp;</span>
	<?php
	if( $paginaAtual > 3 ){
	?>
	<a class="pagina<?=$_POST['req'] ?>" pagina="<?=$pagina ?>" style="background-color:#f5f5f5;color:#006699;BORDER-RIGHT: #a0a0a0 1px solid;text-decoration:none;">&nbsp;<u>1</u>&nbsp;...&nbsp;</a>
	<?php
	}
	foreach( $arrPaginas as $pagina ) {
		if( $pagina == $paginaAtual ){
	?>
	<strong style="background-color:#000000;color:#ffffff;BORDER-RIGHT: #a0a0a0 1px solid;" >&nbsp;<?=$pagina ?>&nbsp;</strong>
	<?php
		}else{
	?>
	<a class="pagina<?=$_POST['req'] ?>" pagina="<?=$pagina ?>" style="background-color:#f5f5f5;color:#006699;BORDER-RIGHT: #a0a0a0 1px solid;text-decoration:none;">&nbsp;<u><?=$pagina ?></u>&nbsp;</a>
	<?php
		}
	}
	if( ($paginas - $paginaAtual) > 3 ){
	?>
	<a class="pagina<?=$_POST['req'] ?>" pagina="<?=$paginas ?>" style="background-color:#f5f5f5;color:#006699;BORDER-RIGHT: #a0a0a0 1px solid;text-decoration:none;">&nbsp;...&nbsp;<u><?=$paginas ?></u>&nbsp;</a>
	<?php
	}
	?>
<?php
}

include_once APPRAIZ . 'www/obras2/_funcoes_obras_par.php';
/*
 * Funções Rejeição obra reformulada
 * */

/* Valida a data no formato YYYY-MM-DD 00:00:00
 * */
function validaData( $data ){

	$arrValidacao = explode(' ', $data);
	$arrValidacao = explode('-', $arrValidacao[0]);

	return checkdate($arrValidacao[1],$arrValidacao[2],$arrValidacao[0]);
}

function retornaBKPObra( $preidAtual, $preidBKP ){

	global $db;

	$sql = "SELECT * FROM obras.preobra WHERE preid = $preidBKP";

	$dados = $db->pegaLinha( $sql );
// 	ver($dados,d);
	extract($dados);

	// criar novo preid (replicar obras.preobra)
	$sql = "UPDATE obras.preobra SET
				presistema = ".($presistema ? $presistema : 'NULL').",
				preidsistema = ".($preidsistema ? $preidsistema : 'NULL').",
				ptoid = ".($ptoid ? $ptoid : 'NULL').",
				preobservacao = ".(trim($preobservacao) ? "'$preobservacao'" : "NULL").",
				prelogradouro = ".(trim($prelogradouro) ? "'$prelogradouro'" : "NULL").",
				precomplemento = ".(trim($precomplemento) ? "'$precomplemento'" : "NULL").",
				estuf = ".(trim($estuf) ? "'$estuf'" : "NULL").",
				muncod = ".(trim($muncod) ? "'$muncod'" : "NULL").",
				precep = ".(trim($precep) ? "'$precep'" : "NULL").",
				prelatitude = ".(trim($prelatitude) ? "'$prelatitude'" : "NULL").",
				prelongitude = ".(trim($prelongitude) ? "'$prelongitude'" : "NULL").",
				predtinclusao = ".(trim($predtinclusao) ? "'$predtinclusao'" : "NULL").",
				prebairro = ".(trim($prebairro) ? "'$prebairro'" : "NULL").",
				preano = ".(trim($preano) ? "'$preano'" : "NULL").",
				qrpid = ".($qrpid ? $qrpid : 'NULL').",
				predescricao = ".(trim($predescricao) ? "'$predescricao'" : "NULL").",
				prenumero = ".($prenumero ? $prenumero : 'NULL').",
				pretipofundacao = ".(trim($pretipofundacao) ? "'$pretipofundacao'" : "NULL").",
				prestatus = ".(trim($prestatus) ? "'$prestatus'" : "NULL").",
				entcodent = ".(trim($entcodent) ? "'$entcodent'" : "NULL").",
				preprioridade = ".(trim($preprioridade) ? "'$preprioridade'" : "NULL").",
				terid = ".($terid ? $terid : 'NULL').",
				resid = ".($resid ? $resid : 'NULL').",
				prevalorobra = ".($prevalorobra ? $prevalorobra : 'NULL').",
				tooid = ".($tooid ? $tooid : 'NULL').",
				muncodpar = ".(trim($muncodpar) ? "'$muncodpar'" : "NULL").",
				estufpar = ".(trim($estufpar) ? "'$estufpar'" : "NULL").",
				premcmv = ".(trim($premcmv) ? "'$premcmv'" : "NULL").",
				predatareformulacao = ".(trim($predatareformulacao) ? "'$predatareformulacao'" : "NULL").",
				preusucpfreformulacao = ".(trim($preusucpfreformulacao) ? "'$preusucpfreformulacao'" : "NULL")."
			WHERE
				preid = $preidAtual";

	$db->executar($sql);
	$db->commit();

	// buscar o qrpid da preobraanalise antiga (recuperar bkp)
	$antigoqrpid = pegaQrpidAnalisePAC( $preidBKP, 49 );

	// criar novo panid (recuperar bkp)
	$sql = "UPDATE obras.preobraanalise SET
				qrpid = $antigoqrpid
			WHERE
				preid = $preidAtual";

	$db->executar($sql);
	$db->commit();

	// Recupera dados Planilha Orçamentária, Cronograma, Fotos e Arquivos
	$sql = '';
	$sqlTeste = "SELECT DISTINCT true
				FROM obras.preplanilhaorcamentaria
				WHERE preid = $preidBKP";
	$testaBKPPlanilha = $db->pegaUm($sqlTeste);
	if( $testaBKPPlanilha == 't' ){
		$sql .= "DELETE FROM obras.preplanilhaorcamentaria WHERE preid = $preidAtual;
				INSERT INTO obras.preplanilhaorcamentaria(preid, itcid, ppovalorunitario)
				(
				SELECT $preidAtual, itcid, ppovalorunitario
				FROM obras.preplanilhaorcamentaria
				WHERE preid = $preidBKP
				);";
	}
	$sqlTeste = "SELECT DISTINCT true
				FROM obras.preobrafotos
				WHERE preid = $preidBKP";
	$testaBKPFotos = $db->pegaUm($sqlTeste);
	if( $testaBKPFotos == 't' ){
		$sql .= "DELETE FROM obras.preobrafotos WHERE preid = $preidAtual;
				INSERT INTO obras.preobrafotos(pofdescricao, preid, arqid)
				(
				SELECT pofdescricao, $preidAtual, arqid
				FROM obras.preobrafotos
				WHERE preid = $preidBKP
				);";
	}
	$sqlTeste = "SELECT DISTINCT true
				FROM obras.preobraanexo
				WHERE preid = $preidBKP";
	$testaBKPAnexo = $db->pegaUm($sqlTeste);
	if( $testaBKPAnexo == 't' ){
		$sql .= "DELETE FROM obras.preobraanexo WHERE preid = $preidAtual;
				INSERT INTO obras.preobraanexo
					(preid, poadescricao, arqid, podid,
					datainclusao, usucpf, poasituacao)
				(
				SELECT
					$preidAtual, poadescricao, arqid, podid,
					datainclusao, usucpf, poasituacao
				FROM obras.preobraanexo
				WHERE preid = $preidBKP
				);";
	}

	$sqlTeste = "SELECT DISTINCT true
				FROM obras.preobradocumentosfnde
				WHERE preid = $preidBKP";
	$testaBKPAnexo = $db->pegaUm($sqlTeste);
	if( $testaBKPAnexo == 't' ){
		$sql .= "DELETE FROM obras.preobradocumentosfnde WHERE preid = $preidAtual;
				INSERT INTO obras.preobradocumentosfnde
					(preid, pdfdescricao, arqid, pdfdatainclusao, pdfstatus)
				(
				SELECT
					$preidAtual, pdfdescricao, arqid, pdfdatainclusao, pdfstatus
				FROM obras.preobradocumentosfnde
				WHERE preid = $preidBKP
				);";
	}

	$sqlTeste = "SELECT DISTINCT true
				FROM obras.precronograma
				WHERE preid = $preidBKP";
	$testaBKPCronograma = $db->pegaUm($sqlTeste);
	if( $testaBKPCronograma == 't' ){
		$sql .= "DELETE FROM obras.precronograma WHERE preid = $preidAtual;
				INSERT INTO obras.precronograma( itcid, preid, prcmes, prcquinzena )
				(
				SELECT itcid, $preidAtual, prcmes, prcquinzena
				FROM obras.precronograma
				WHERE preid = $preidBKP
				);";
	}
	// FIM - Recupera dados Planilha Orçamentária, Cronograma Fotos e Arquivos

	$db->executar($sql);
	$db->commit();

}

/**
 * Busca lista de Escolas com itens que possuem quantidade informada pelo técnico e gera um arquivo fisico PDF com o
 * relatório
 *
 * @param array $listaSubacoes Lista com subações para consultar escolas e itens que possuem quantidade
 * @return VOID
 */
function gerarRelatorioEscolaItenPorQuantidade(array $subacoes, $dopid){
    Global $db;

    if(is_array($subacoes) && $subacoes[0]){
        $tipoEscola = $db->pegaUm("SELECT count(sbacronograma) FROM par.subacao s INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid WHERE s.sbacronograma = 2 AND sd.sbdid IN (".implode(",", $subacoes).")");
        if($tipoEscola > 0){
            $sqlRelatorio = "
                SELECT * FROM (
                    SELECT
                        CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE mun.estuf END as uf,
                        CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE mun.mundescricao END as entidade,
                        ent.entnome as escola,
                        ent.entcodent as codinep,
                        (par.retornacodigosubacao(sd.sbaid)) as subacao,
                        pic.picdescricao as item,
                        CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
                            THEN -- escolas sem itens
                                sum(coalesce(se.sesquantidadetecnico,0) * coalesce(sic.icovalor,0))::numeric(20,2)
                            ELSE -- escolas com itens
                                CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
                                    sum(ssi.seiqtdtecnico)
                                END
                        END as quantidade
                    FROM
                        par.subacaodetalhe sd
                    INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
                    INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sd.sbaid AND sic.icoano = sd.sbdano
                    INNER JOIN par.propostaitemcomposicao pic ON pic.picid = sic.picid
                    INNER JOIN par.subacaoescolas se ON se.sbaid = sd.sbaid AND se.sesano = sd.sbdano
                    INNER JOIN par.escolas esc on esc.escid = se.escid
                    INNER JOIN entidade.entidade ent ON ent.entid = esc.entid
                    INNER JOIN par.subescolas_subitenscomposicao ssi ON ssi.icoid = sic.icoid AND ssi.sesid = se.sesid
                    INNER JOIN par.acao a ON a.aciid = s.aciid
                    INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
                    INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
                    LEFT JOIN territorios.municipio mun ON mun.muncod = iu.muncod
                    WHERE
                        sd.sbdid IN (".implode(",", $subacoes).")
                    GROUP BY
                        ent.entnome, ent.entcodent, sd.sbaid, pic.picdescricao, s.frmid, s.ptsid, sic.icovalidatecnico, iu.itrid, iu.estuf, mun.estuf, mun.mundescricao
                    ORDER BY
                        escola, subacao, item
                ) foo
                WHERE
                    foo.quantidade > 0 ";
            $relatorio = $db->carregar($sqlRelatorio);
            if( $relatorio ) geraAnexoEscolas($relatorio, $dopid);
        }
    }
}

/**
 * Cria filtro para buscar processos que possuem ou não pendencia de acordo com
 * opções passadas para essa função
 *
 * @param string $optionPendenciaObras 1 - Busca processos com pendência 2 - Busca processos sem pendências
 * @return string Retorna o sql referente a clausula where para filtrar processos com pendencias
 */
function buscarSelectProcessoPendencia($optionPendenciaObras){
    $arrObrasPendentesM = bloqueioObra('','','','Municipal');
    $arrObrasPendentesE = bloqueioObra('','','','Estadual');
    $sqlProPendencia = "
        SELECT DISTINCT
            pendencia_p.proid
        FROM
            par.processoobraspar AS pendencia_p
        WHERE
        	pendencia_p.prostatus = 'A' and
            pendencia_p.muncod IN ('".implode("','", $arrObrasPendentesM)."')
            OR
            pendencia_p.estuf IN ('".implode("','", $arrObrasPendentesE)."')
    ";

    if($optionPendenciaObras == '1'){
        $operador = 'IN';
    } else {
        $operador = 'NOT IN';
    }

    return "p.proid $operador( $sqlProPendencia )";
}

/**
 * Cria filtro para buscar processos que possuem ou não pendencia de acordo com
 * opções passadas para essa função
 *
 * @param string $optionPendenciaObras 1 - Busca processos com pendência 2 - Busca processos sem pendências
 * @return string Retorna o sql referente a clausula where para filtrar processos com pendencias
 */
function buscarSelectProcessoParPendencia($optionPendenciaObras){
    $arrObrasPendentesM = bloqueioObra('','','','Municipal');
    $arrObrasPendentesE = bloqueioObra('','','','Estadual');
    $sqlProPendencia = "
        SELECT DISTINCT
            pendencia_p.prpid
        FROM
            par.processopar AS pendencia_p
        WHERE
            pendencia_p.prpstatus = 'A' and pendencia_p.muncod IN ('".implode("','", $arrObrasPendentesM)."')
    ";

    if($optionPendenciaObras == '1'){
        $operador = 'IN';
    } else {
        $operador = 'NOT IN';
    }

    return "p.prpid $operador( $sqlProPendencia )";
}

/**
 * Cria filtro para buscar pre obras que possuem ou não pendencia de acordo com
 * opções passadas para essa função
 *
 * @param string $optionPendenciaObras 1 - Busca processos com pendência 2 - Busca processos sem pendências
 * @return string Retorna o sql referente a clausula where para filtrar processos com pendencias
 */
function buscarSelectPreObraPendencia($optionPendenciaObras){
    $arrObrasPendentesM = bloqueioObra('','','','Municipal');
    $arrObrasPendentesE = bloqueioObra('','','','Estadual');
    $sqlProPendencia = "
        SELECT DISTINCT
            pendencia_p.preid
        FROM
            obras.preobra AS pendencia_p
        WHERE
            (
                (
                    pendencia_p.preesfera = 'M'
                    AND
                    pendencia_p.muncod IN ( '".implode("','", $arrObrasPendentesM)."' )
                )
                OR
                (
                    pendencia_p.preesfera = 'E'
                    AND pendencia_p.estuf IN ( '".implode("','", $arrObrasPendentesE)."' )
                )
            )
    ";

    if($optionPendenciaObras == '1'){
        $operador = 'IN';
    } else {
        $operador = 'NOT IN';
    }

    return "AND pre.preid $operador( $sqlProPendencia )";
}

function criaBKPObra( $preid ){

	global $db;

	$sql = "UPDATE obras.preobra SET predatareformulacao=NOW(), preusucpfreformulacao='".$_SESSION['usucpf']."' WHERE preid = $preid ";
	$db->executar($sql);


	// criar novo preid (replicar obras.preobra)
	$sql = "INSERT INTO obras.preobra(
				docid, presistema, preidsistema, ptoid, preobservacao,
				prelogradouro, precomplemento, estuf, muncod, precep, prelatitude,
				prelongitude, predtinclusao, prebairro, preano, qrpid, predescricao,
				prenumero, pretipofundacao, prestatus, entcodent, preprioridade,
				terid, resid, prevalorobra, tooid, muncodpar, estufpar, premcmv,
				preidpai, predatareformulacao, preusucpfreformulacao)
			(
			SELECT
				NULL, presistema, preidsistema, ptoid, preobservacao,
				prelogradouro, precomplemento, estuf, muncod, precep, prelatitude,
				prelongitude, predtinclusao, prebairro, preano, NULL, predescricao,
				prenumero, pretipofundacao, prestatus, entcodent, preprioridade,
				terid, resid, prevalorobra, tooid, muncodpar, estufpar, premcmv,
				$preid, predatareformulacao, preusucpfreformulacao
			FROM obras.preobra
			WHERE preid = $preid
			)
			RETURNING preid";

	$novopreid = $db->pegaUm($sql);

	// buscar o qrpid da preobraanalise antiga
	$sql = "SELECT qrpid FROM obras.preobraanalise WHERE preid = $preid";
	$antigoqrpid = $db->pegaUm($sql);

	// buscar o qrpid da preobra2 antiga
	$sql = "SELECT qrpid FROM obras.preobra WHERE preid = $preid";
	$antigoqrpid2 = $db->pegaUm($sql);

	// criar novo qrpid (replicar questionario.questionarioresposta)
	$sql = "INSERT INTO questionario.questionarioresposta(
				queid, qrptitulo, qrpdata)
			(
				SELECT queid, 'OBRAS (".$novopreid." - ".$mundescricao.")', qrpdata
				FROM questionario.questionarioresposta
				WHERE qrpid = $antigoqrpid
			)
			RETURNING qrpid";
	$novoqrpid = $db->pegaUm($sql);

	// pegando descricao o municipio
	$sql = "SELECT m.mundescricao
			FROM obras.preobra p
			LEFT JOIN territorios.municipio m ON m.muncod = p.muncod
			WHERE preid = $preid";
	$mundescricao = $db->pegaUm($sql);

	// criar novo qrpid (replicar questionario.questionarioresposta)
	$sql = "INSERT INTO questionario.questionarioresposta(queid, qrptitulo, qrpdata)
			(
				SELECT queid, 'OBRAS (".$novopreid." - ".$mundescricao.")', qrpdata
				FROM questionario.questionarioresposta
				WHERE qrpid='".$antigoqrpid2."'
			)
			RETURNING qrpid";
	$novoqrpid2 = $db->pegaUm($sql);

	$sql = "UPDATE obras.preobra SET qrpid='".$novoqrpid2."' WHERE preid='".$novopreid."';
			INSERT INTO obras.preobraanalise(
				preid, poadataanalise, poastatus, poausucpfinclusao, qrpid, poaindeferido,
				poajustificativa)
			(
				SELECT
					$novopreid, poadataanalise, poastatus, poausucpfinclusao,
					".(($novoqrpid)?"'".$novoqrpid."'":"NULL").", poaindeferido,
					poajustificativa
				FROM obras.preobraanalise
				WHERE preid = $preid
			);
			INSERT INTO questionario.resposta(perid, qrpid, usucpf, itpid, resdsc)
			(
				SELECT perid, $novoqrpid, usucpf, itpid, resdsc
				FROM questionario.resposta
				WHERE qrpid = $antigoqrpid
			);
			INSERT INTO questionario.resposta(perid, qrpid, usucpf, itpid, resdsc)
			(
				SELECT perid, $novoqrpid2, usucpf, itpid, resdsc
				FROM questionario.resposta
				WHERE qrpid = $antigoqrpid2
			);
			INSERT INTO obras.preobrafotos(pofdescricao, preid, arqid)
			(
				SELECT pofdescricao, $novopreid, arqid
				FROM obras.preobrafotos
				WHERE preid = $preid
			);
			INSERT INTO obras.preobraanexo(
				preid, poadescricao, arqid, podid, datainclusao, usucpf,
				poasituacao)
			(
			SELECT
				$novopreid, poadescricao, arqid, podid, datainclusao, usucpf,
				poasituacao
			FROM obras.preobraanexo
			WHERE preid = $preid
			);
			INSERT INTO obras.preobradocumentosfnde(
            preid, pdfdescricao, arqid, pdfdatainclusao, pdfstatus)
            (
	            SELECT $novopreid, pdfdescricao, arqid, pdfdatainclusao, pdfstatus
				FROM obras.preobradocumentosfnde
				WHERE preid = $preid
			);
			INSERT INTO obras.preplanilhaorcamentaria(preid, itcid, ppovalorunitario)
			(
				SELECT $novopreid, itcid, ppovalorunitario
				FROM obras.preplanilhaorcamentaria
				WHERE preid = $preid
			);
			INSERT INTO obras.precronograma( itcid, preid, prcmes, prcquinzena )
			(
				SELECT itcid, $novopreid, prcmes, prcquinzena
				FROM obras.precronograma
				WHERE preid = $preid
			)";
	$db->executar($sql);
	$db->commit();

	return $novopreid;
}

function wf_pos_cancelaReformulacao( $preid ){

	global $db;

// 	if( !verificaAnoMeta( $preid ) ){
// 		return false;
// 	}

	$preid = $_REQUEST['preid'] ? $_REQUEST['preid'] : $preid;

	$novopreid = criaBKPObra( $preid );

	$sql = "INSERT INTO obras.preobracancelamentoreformulacao(preid, pcrmotivo, pcrdata, usucpf)
			VALUES ($novopreid, '{$_REQUEST['cmddsc']}', now(), {$_SESSION['usucpf']})";

	$db->executar($sql);
	$db->commit();

	$sql = "SELECT
				max(preid)
			FROM
				obras.preobra
			WHERE
				preidpai = $preid
				AND preid NOT IN
								(	SELECT preid
									FROM obras.preobracancelamentoreformulacao
									WHERE pcrstatus = 'A'
								)";

	$preidRefAprovada = $db->pegaUm($sql);

	if( $preidRefAprovada ){
		retornaBKPObra( $preid, $preidRefAprovada );
	}

	$sql = "SELECT
				COALESCE(obrpercentultvistoria, 0) as percexec,
				obrid,
				doc.esdid,
				doc.docid,
				(
				SELECT
					aed.esdidorigem
				FROM
					workflow.historicodocumento 	hst
				INNER JOIN workflow.acaoestadodoc  	aed ON aed.aedid = hst.aedid
				WHERE
					hst.hstid = (SELECT max(hstid) FROM workflow.historicodocumento WHERE docid = doc.docid)
				) as estadoorigem
			FROM
				obras2.obras obr
			INNER JOIN workflow.documento doc ON doc.docid = obr.docid
			WHERE
				preid = $preid
				AND obrstatus = 'A'";

	$arObra = $db->pegaLinha( $sql );

	if( $arObra['obrid'] != '' && $arObra['estadoorigem'] != '' ){

		$sql = "SELECT
					aedid
				FROM workflow.acaoestadodoc
				WHERE
					esdiddestino = {$arObra['estadoorigem']}
					AND esdidorigem = {$arObra['esdid']}";

		$aedid = $db->pegaUm($sql);

		if( $aedid == '' ){
			$sql = "INSERT INTO workflow.acaoestadodoc
						(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada,
						esdsncomentario, aedvisivel, aedcodicaonegativa)
					VALUES(
						{$arObra['esdid']},
						{$arObra['estadoorigem']}, 'Enviar para estado anterior', 'A', 'Enviada para estado anterior',
						true, false, false )
					RETURNING
						aedid";

			$aedid = $db->pegaUm($sql);
		}

		$teste = wf_alterarEstado( $arObra['docid'], $aedid, 'Tramitado por wf_pos_cancelaReformulacao preid = '.$preid, array( 'docid' => $arObra['docid'] ) );
	}

	$sql = "SELECT tooid FROM obras.preobra WHERE preid = $preid";

	$tooid = $db->pegaUm( $sql );

	if( $tooid == '1' ){

		$sql = "UPDATE obras.preobra SET prereformulacao = NULL WHERE preid = $preid";

		$db->executar( $sql );
		$db->commit();
	}

	return true;
}

/**
 *
 * @global type $db
 * @param type $id
 */
function possui_perfil( $pflcods ){

	global $db;
	/*
	if( $db->testa_superuser() ){
		return true;
	}
		*/

	if ( is_array( $pflcods ) )
	{
		$pflcods = array_map( "intval", $pflcods );
		$pflcods = array_unique( $pflcods );
	}
	else
	{
		$pflcods = array( (integer) $pflcods );
	}
	if ( count( $pflcods ) == 0 )
	{
		return false;
	}
	$sql = "SELECT
				count(*)
			FROM
				seguranca.perfilusuario
			WHERE
				usucpf = '" . $_SESSION['usucpf'] . "'
				AND pflcod in ( " . implode( ",", $pflcods ) . " ) ";
	return $db->pegaUm( $sql ) > 0;
}

function pegaEstadoAtualDocumento( $docid ) {
	global $db;
	if($docid) {
		$docid = (integer) $docid;
		$sql = "SELECT			esdid
				FROM			workflow.documento
				WHERE			docid = {$docid}";
		$estado = $db->pegaUm( $sql );
		return $estado;
	} else {
		return false;
	}
}

function testaAtendimentoDemandaPorId( $dmdid ){

	global $db;

	$arrEstados = Array(PAR_WF_DEMANDA_ESTADO_EM_CADASTRAMENTO,
			PAR_WF_DEMANDA_ESTADO_AGUARDANDO_ATENDIMENTO,
			PAR_WF_DEMANDA_ESTADO_EM_DILIGENCIA);

	$sql = "SELECT
				true
			FROM
				par.demanda dmd
			INNER JOIN workflow.documento doc ON doc.docid = dmd.docid
			WHERE
				dmdid = $dmdid
				AND dmdstatus = 'A'
				AND esdid NOT IN (".implode(', ',$arrEstados).")";

	$atendimento = $db->pegaUm($sql);

	return $atendimento == 't';
}

function testaAtendimentoDemandaPorPrioridade(){

	global $db;

	$arrEstados = Array(PAR_WF_DEMANDA_ESTADO_EM_CADASTRAMENTO,
					    PAR_WF_DEMANDA_ESTADO_AGUARDANDO_ATENDIMENTO,
						PAR_WF_DEMANDA_ESTADO_EM_DILIGENCIA);

	$sql = "SELECT
				true
			FROM
				par.demanda dmd
			INNER JOIN workflow.documento doc ON doc.docid = dmd.docid
			WHERE
				dmtid = {$_REQUEST['dmtid']}
				AND dmdprioridade = {$_REQUEST['dmdprioridade']}
				AND dmdstatus = 'A'
				AND esdid NOT IN (".implode(', ',$arrEstados).")";

	$atendimento = $db->pegaUm($sql);

	echo $atendimento;
	return $atendimento;
}

function pegaPrioridadeMinimaSemAtendimento(){

	global $db;

	$arrEstados = Array(PAR_WF_DEMANDA_ESTADO_EM_CADASTRAMENTO,
					    PAR_WF_DEMANDA_ESTADO_AGUARDANDO_ATENDIMENTO,
						PAR_WF_DEMANDA_ESTADO_EM_DILIGENCIA);

	$sql = "SELECT
				min(dmdprioridade) as prioridade
			FROM
				par.demanda dmd
			INNER JOIN workflow.documento doc ON doc.docid = dmd.docid
			WHERE
				dmtid = {$_REQUEST['dmtid']}
				AND dmdprioridade > {$_REQUEST['prioridadeInvalida']}
				AND dmdstatus = 'A'
				AND esdid IN (".implode(', ',$arrEstados).")";

	$prioridade = $db->pegaUm($sql);

	if( $prioridade == '' ){

		$sql = "SELECT
					max(dmdprioridade)+1 as prioridade
				FROM
					par.demanda dmd
				INNER JOIN workflow.documento doc ON doc.docid = dmd.docid
				WHERE
					dmtid = {$_REQUEST['dmtid']}
					AND dmdstatus = 'A'
					AND esdid NOT IN (".implode(', ',$arrEstados).")";

		$prioridade = $db->pegaUm($sql);

		if( $prioridade == '' ){
			$prioridade = 0;
		}
	}
	echo $prioridade;
	return $prioridade;
}

function pegaPrioridadeMinima(){

	global $db;

	$arrEstados = Array(PAR_WF_DEMANDA_ESTADO_EM_CADASTRAMENTO,
					    PAR_WF_DEMANDA_ESTADO_AGUARDANDO_ATENDIMENTO,
						PAR_WF_DEMANDA_ESTADO_EM_DILIGENCIA);

	$sql = "SELECT
				min(dmdprioridade) as prioridade
			FROM
				par.demanda dmd
			INNER JOIN workflow.documento doc ON doc.docid = dmd.docid
			WHERE
				dmtid = {$_REQUEST['dmtid']}
				AND dmdstatus = 'A'
				AND esdid IN (".implode(', ',$arrEstados).")";

	$prioridade = $db->pegaUm($sql);

	if( $prioridade == '' ){

		$sql = "SELECT
					max(dmdprioridade)+1 as prioridade
				FROM
					par.demanda dmd
				INNER JOIN workflow.documento doc ON doc.docid = dmd.docid
				WHERE
					dmtid = {$_REQUEST['dmtid']}
					AND dmdstatus = 'A'
					AND esdid NOT IN (".implode(', ',$arrEstados).")";

		$prioridade = $db->pegaUm($sql);

		if( $prioridade == '' ){
			$prioridade = 0;
		}
	}
	echo $prioridade;
	return $prioridade;
}

function pegaPrioridadeMaxima(){

	global $db;

	$sql = "
	    SELECT
            MAX(dmdprioridade) as prioridade
        FROM
            par.demanda
        WHERE
            dmdstatus = 'A';
    ";

	$prioridade = $db->pegaUm($sql);



	if( $prioridade == '' ){
		$prioridade = 0;
	}
	echo $prioridade;
	return $prioridade;
}

function pegaDocidDemanda( $dmdid ){
	global $db;

	require_once APPRAIZ . 'includes/workflow.php';

	// descrição do documento
	$docdsc = "Fluxo de demandas do PAR - dmdid " . $dmdid;

	// cria documento do WORKFLOW
	$docid = wf_cadastrarDocumento( WF_TPDID_DEMANDA, $docdsc );

	// atualiza o DOCID na OBRA
	$sql = "UPDATE par.demanda SET
				docid = $docid
			WHERE
				dmdid = $dmdid";
	$db->executar($sql);
	$db->commit();

	return $docid;
}

function ajustaProxDemandaAtendimento( $demandas, $k ){

	global $db;

	$arrEstados = Array(PAR_WF_DEMANDA_ESTADO_EM_CADASTRAMENTO,
						PAR_WF_DEMANDA_ESTADO_AGUARDANDO_ATENDIMENTO,
						PAR_WF_DEMANDA_ESTADO_EM_DILIGENCIA);

	if( !in_array($demandas[$k]['esdid'], $arrEstados) && $demandas[$k]['esdid'] != '' ){
		return ajustaProxDemandaAtendimento( $demandas, $k+1 )+1;
	}else{
		return '1';
	}
}

function limpaPrioridade( $dmdid ){

	global $db;

	$sql = "UPDATE par.demanda SET
				dmdprioridade = NULL
			WHERE
				dmdid = $dmdid";

	$db->executar($sql);
	$db->commit();
}

function atualizaPrioridadeDemanda( $dmdprioridade, $dmtid, $tiraPrioridade = false ){

	global $db;

	if( $tiraPrioridade ){
		$operador = '-';
		$operadorWhere = ">";
	}else{
		$operador = '+';
		$operadorWhere = ">=";
	}

	$arrEstados = Array(PAR_WF_DEMANDA_ESTADO_EM_CADASTRAMENTO,
						PAR_WF_DEMANDA_ESTADO_AGUARDANDO_ATENDIMENTO,
						PAR_WF_DEMANDA_ESTADO_EM_DILIGENCIA);

	$sql = "SELECT
				dmdid,
				esdid,
				dmdprioridade
			FROM
				par.demanda dmd
			INNER JOIN workflow.documento doc ON doc.docid = dmd.docid
			WHERE
				dmdstatus = 'A'
				AND dmdprioridade IS NOT NULL
				AND dmdprioridade $operadorWhere $dmdprioridade
				--AND dmtid = $dmtid
			ORDER BY
				dmdprioridade";

	$demandas = $db->carregar($sql);

	if( is_array($demandas) ){
		$sql = "";
		foreach( $demandas as $k => $demanda ){
			if( !in_array($demanda['esdid'], $arrEstados) ){
				continue;
			}
			$interacao = ajustaProxDemandaAtendimento( $demandas, $k+1 );
			$sql .= "UPDATE par.demanda SET
						dmdprioridade = dmdprioridade $operador $interacao
					WHERE
						dmdid = {$demanda['dmdid']};";
		}
		if( $sql != '' ){
			$db->executar($sql);
			$db->commit();
		}
	}

}

function atualizaEfeitoPriorizacao($prioridade, $hpdid, $dmdid){

	global $db;

	$sql = "SELECT dmdid FROM par.demanda WHERE dmdprioridade >= $prioridade OR dmdid = $dmdid";

	$dmdids = $db->carregarColuna($sql);

	$sql = "";
	if( is_array($dmdids) ){
		foreach( $dmdids as $dmdid ){
			$sql .= "INSERT INTO par.demanda_priorizacao_demanda(dmdid, hpdid)
					 VALUES ($dmdid, $hpdid);";
		}
	}
	if( $sql != "" ){
		$db->executar($sql);
		$db->commit();
	}
}

function tiraFuroOrdem( $dmtid ){

	global $db;

	$sql = "SELECT
				dmdid
			FROM
				par.demanda
			WHERE
				dmdprioridade IS NOT NULL
				AND dmdstatus = 'A'
				--AND dmtid = $dmtid
			ORDER BY
				dmdprioridade,
				dmdultimaalteracao DESC";
	$dmdids = $db->carregarColuna($sql);

	if( is_array($dmdids) ){
		$sql = "";
		foreach( $dmdids as $k => $dmdid ){
			$sql .= "UPDATE par.demanda SET dmdprioridade = ".($k+1)." WHERE dmdid = $dmdid;";
		}
		$db->executar($sql);
		$db->commit();
	}
}

function carregarDemandaPorId($id)
{
    global $db;
    $sql = "SELECT * FROM par.demanda WHERE dmdid ={$id}";
    $resul = $db->pegaLinha($sql);
    foreach($resul as &$value) $value = utf8_encode($value);

    echo simec_json_encode($resul);
    exit;
}

function desativarDemanda($id)
{
    global $db;

//    $sql = "DELETE FROM par.demanda WHERE dmdid = {$id} ";
    $sql = "UPDATE par.demanda
            SET dmdstatus = 'I'
            WHERE dmdid = {$id}";
    $db->executar($sql);
    $db->commit();

    $return = array('status' => true , 'name' => 'dmdnome' , 'msg' => utf8_encode('Demanda excluida com sucesso!'), 'result' => $id);
    echo simec_json_encode($return);
    exit;
}

function carregarImagensDemanda($dmdid , $status = 'A')
{
    global $db;
    $sql = "SELECT * FROM par.demandaanexo WHERE dmdid = {$dmdid} and aqsstatus = '{$status}'";
    $imagensDemanda = $db->carregar($sql);
    return ($imagensDemanda)? $imagensDemanda : array();
}

function carregarHistoricoDemanda($dmdid)
{
    global $db;
    $sql = "SELECT *, TO_CHAR(dmddatacadastro , 'DD/MM/YYYY') as data FROM par.demanda d
            INNER JOIN par.demandatipo dt on d.dmtid = dt.dmtid WHERE dmdidpai = {$dmdid}";
    $historicosDemanda = $db->carregar($sql);
//    foreach($historicosDemanda as &$historicoDemanda)
//    foreach($historicoDemanda as &$campoDemanda)
//        $campoDemanda = utf8_encode($campoDemanda);

    return ($historicosDemanda)? $historicosDemanda : array();
}

function gerarHistoricoDemanda($dmdid)
{
//    echo "<script>alert('teste');</script>";
    global $db;

    $sql = "SELECT * FROM par.demanda WHERE dmdid ={$dmdid}";

    $demanda = $db->pegaLinha($sql);
    $sql = "INSERT INTO par.demanda ( dmdnome , dmddescricao , dmtid , dmdprioridade , docid , dmdcpfcadastro , dmddatacadastro , dmdstatus , dmdidpai )
            VALUES (
                    '{$demanda['dmdnome']}' ,
                    '{$demanda['dmddescricao']}' ,
                    {$demanda['dmtid']}  ,
                    {$demanda['dmdprioridade']} ,
                    {$demanda['docid']} ,
                    '{$demanda['dmdcpfcadastro']}' ,
                    now() ,
                    'I' ,
                    {$dmdid})
            RETURNING dmdid";
    $dmdidNew = $db->pegaUm($sql);

    $imagensDemanda = carregarImagensDemanda($dmdid);

    foreach($imagensDemanda as $imagemDemanda){
        $sql = "INSERT INTO par.demandaanexo (dmdid , arqid , aqsstatus)
            VALUES ({$dmdidNew} , {$imagemDemanda['arqid']} , '{$imagemDemanda['aqsstatus']}')
            RETURNING dmdid || '_' || arqid";

            $imgIdNew[] = $db->pegaUm($sql);
    }

    if($dmdidNew){
        $db->commit();
        return true;
    } else {
        $db->rollback();
        echo "<script>msg('". MSG002.", pois n?o pode gerar hist?rico da demanda!')</script>";
        return false;
    }
}

function salvarDemanda($data)
{
    global $db;

    require_once APPRAIZ . 'includes/workflow.php';

    //Correcao de charset para inserir no banco
    foreach($data as &$value) if(!empty($value)) $value = utf8_decode($value);

    if($data['dmdid']){
        //Opera??o de edi??o da demanda.
        $sql = "UPDATE par.demanda
                SET
                    dmtid = {$data['dmtid']} ,
                    dmdprioridade = {$data['dmdprioridade']} ,
                    dmddescricao = '{$data['dmddescricao']}'
                WHERE dmdid = {$data['dmdid']}
                RETURNING dmdid";

    } else {
        // Oprea??o de cadastro da demanda.

        //Verifica se ja tem demanda com este assunto.
        $sql = "SELECT * FROM par.demanda WHERE TRIM(LOWER(dmdnome)) = TRIM(LOWER('{$data['dmdnome']}')) AND dmdstatus = 'A' ";
        $demanda = $db->pegaLinha($sql);
        if($demanda){
            $return = array('status' => false , 'name' => 'dmdnome' , 'msg' => utf8_encode('Ja existe uma demanda com este assunto!'), 'result' => $demanda['dmdid']);
            echo simec_json_encode($return);
            exit;
        }

        $data['dmdcpfcadastro'] = $_SESSION['usucpf'];

        // recupera o tipo do documento
        $tpdid = WF_TPDID_DEMANDA;

        // descri??o do documento
        $docdsc = $data['dmdnome'];

        // cria documento do WORKFLOW
        $docid = wf_cadastrarDocumento( $tpdid, $docdsc );

        $sql = "INSERT INTO par.demanda ( dmdnome , dmddescricao , dmtid , dmdprioridade , docid , dmdcpfcadastro , dmddatacadastro )
            VALUES ('{$data['dmdnome']}' , '{$data['dmddescricao']}' , {$data['dmtid']}  , {$data['dmdprioridade']} , {$docid} , '{$data['dmdcpfcadastro']}' , now())
            RETURNING dmdid";
    }

    $dmdid = $db->pegaUm($sql);
    $db->commit();

    if($dmdid) $return = array('status' => true , 'msg' => utf8_encode(MSG001), 'result' => $dmdid);
    else $return = array('status' => false , 'msg' => utf8_encode(MSG002));

    echo simec_json_encode($return);
    exit;
}

function pegaResponssabilidade( $tprcod ){

	global $db;

	$perfil = pegaPerfilGeral();
	$perfil = $perfil ? $perfil : array();

	if($tprcod == '1'){//Lista Estados
		#Verifica se o perfil é de cunsulta geral e equipe financeira, cunsulta geral e equipe técnica. Caso seja, mostra todos os estados, sem restrição, idependente de sua responsabilidade. Se não da continuidade ao processo "de forma normal".
		if(	in_array(PAR_PERFIL_EQUIPE_FINANCEIRA,$perfil) ||
			in_array(PAR_PERFIL_EQUIPE_TECNICA,$perfil)
		){
			$sql = "SELECT estuf FROM par.instrumentounidade where estuf is not null;";
			$r = $db->carregarColuna($sql);
			return $r;
		}else{
			$sql = "SELECT estuf FROM par.usuarioresponsabilidade WHERE usucpf = '".$_SESSION['usucpf']."' AND rpustatus = 'A'";
		}
	}elseif($tprcod == '2'){//Lista Município
		#Verifica se o perfil é de cunsulta geral e equipe financeira, cunsulta geral e equipe técnica. Caso seja, mostra todos os estados, sem restrição, idependente de sua responsabilidade. Se não da continuidade ao processo "de forma normal".
		if(	in_array(PAR_PERFIL_EQUIPE_FINANCEIRA,$perfil) ||
			in_array(PAR_PERFIL_PROFUNC_ANALISEPF,$perfil) ||
			in_array(PAR_PERFIL_EQUIPE_TECNICA,$perfil)
		){
			$sql = "SELECT muncod FROM par.instrumentounidade where muncod is not null;";
			$r = $db->carregarColuna($sql);
			return $r;
		}else{
			$sql = "SELECT muncod FROM par.usuarioresponsabilidade WHERE usucpf = '".$_SESSION['usucpf']."' AND rpustatus = 'A'";
		}
	}

	$r = $db->carregarColuna($sql);
	if( in_array(PAR_PERFIL_EQUIPE_TECNICA,$perfil) ||
		in_array(PAR_PERFIL_EQUIPE_FINANCEIRA,$perfil) ||
		in_array(PAR_PERFIL_EQUIPE_MUNICIPAL,$perfil) ||
		in_array(PAR_PERFIL_EQUIPE_ESTADUAL,$perfil) ||
		in_array(PAR_PERFIL_CONSULTA_ESTADUAL,$perfil) ||
        in_array(PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL,$perfil) ||
		in_array(PAR_PERFIL_CONSULTA_MUNICIPAL,$perfil) ||
        in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL,$perfil) ||
		in_array(PAR_PERFIL_PREFEITO,$perfil) ||
		in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,$perfil) ||
		in_array(PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO,$perfil) ||
		in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$perfil) ||
		in_array(PAR_PERFIL_ANALISTA_MERITOS,$perfil) ){
		$r = $r ? $r : Array('NULL');
	}
	return $r;
}

function funcaoLinkPronatec()
{
	global $db;

	$muncodsPronatec = $db->carregarColuna("SELECT DISTINCT muncod FROM par.termopronatec");

	if( in_array($_SESSION['par']['muncod'], $muncodsPronatec) )
		return true;
	else
		return false;
}

function mensagemAcossiacao($boNomeTipo){
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloCentro" align="center"><font color="red"><?php echo 'É necessário Associar a um '.$boNomeTipo; ?></font></td>
	</tr>
</table>
	<?php
	die;
}

function criaAbaPar($bEstruturaAvaliacao = false){
	if(!$_SESSION['par']['boAbaMunicipio'] && !$_SESSION['par']['boAbaEstado']){
		$abasPar = array( 0 => array( "descricao" => "Lista de Estados", "link"	  => "par.php?modulo=principal/listaEstados&acao=A"),
						  1 => array( "descricao" => "Lista de Municípios", "link"	  => "par.php?modulo=principal/listaMunicipios&acao=A")
						 );
	} elseif($_SESSION['par']['boAbaMunicipio']){
		$abasPar = array(
						  0 => array( "descricao" => "Lista de Municípios", "link"	  => "par.php?modulo=principal/listaMunicipios&acao=A" )
						 );
	} elseif($_SESSION['par']['boAbaEstado']){
		$abasPar = array(
						  0 => array( "descricao" => "Lista de Estados", "link"	  => "par.php?modulo=principal/listaEstados&acao=A" )
						 );
	}
	return $abasPar;
}

function criaAbaListaObrasPar(){
	$abasPar = array(
	    0 => array(
	        "descricao" => "Lista de Obras PAR/PAC",
	        "link" => "par.php?modulo=principal/listaObrasParUnidade&acao=A"
        ),
        1 => array(
            "descricao" => "Lista de Obras em Diligência",
            "link" => "par.php?modulo=principal/listaObrasEmDiligencia&acao=A"
        )
    );

	return $abasPar;
}

function criaAbaParApoio($bEstruturaAvaliacao = false){
	$abasPar = array( 1 => array( "descricao" => "Plano de Apoio",
									  "link"	  => "par.php?modulo=principal/planoDeApoio&acao=A" ),
		  			  0 => array( "descricao" => "Lista de Professores",
									  "link"	  => "par.php?modulo=principal/listaProfessores&acao=A" )
		 );

	return $abasPar;
}

function carregaAbaDadosUnidade($stPaginaAtual = null, $itrid = null)
{
	$itrid = $itrid ? $itrid : $_SESSION['par']['itrid'];

	$abas = array();

	if($itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL)
	{
		array_push($abas, array("id" => 0, "descricao" => "Árvore", "link" => "/par/par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore"));
		array_push($abas, array("id" => 1, "descricao" => "Prefeitura", "link" => "/par/par.php?modulo=principal/orgaoEducacao&acao=A&funid=1"));
		array_push($abas, array("id" => 2, "descricao" => "Prefeito(a)", "link" => "/par/par.php?modulo=principal/orgaoEducacao&acao=A&funid=2"));
		array_push($abas, array("id" => 3, "descricao" => "Secretaria Municipal de Educação", "link" => "/par/par.php?modulo=principal/orgaoEducacao&acao=A&funid=7"));
		array_push($abas, array("id" => 4, "descricao" => "Dirigente Municipal de Educação", "link" => "/par/par.php?modulo=principal/orgaoEducacao&acao=A&funid=15"));
		array_push($abas, array("id" => 5, "descricao" => "CACS", "link" => "/par/par.php?modulo=principal/cacs&acao=A"));
	}
	else
	{
//		array_push($abas, array("id" => 0, "descricao" => "Programa", "link" => "/par/par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa"));
		array_push($abas, array("id" => 1, "descricao" => "Secretaria Estadual de Educação", "link" => "/par/par.php?modulo=principal/orgaoEducacao&acao=A&funid=6"));
		array_push($abas, array("id" => 2, "descricao" => "Secretário(a) Estadual de Educação", "link" => "/par/par.php?modulo=principal/orgaoEducacao&acao=A&funid=25"));
//		array_push($abas, array("id" => 2, "descricao" => " Governo Estadual ", "link" => "/par/par.php?modulo=principal/orgaoEducacao&acao=A"));
//		array_push($abas, array("id" => 3, "descricao" => "Secretário(a) Estadual de Educação", "link" => "/par/par.php?modulo=principal/dirigenteEducacao&acao=A"));
	}

	array_push($abas, array("id" => 6, "descricao" => "Equipe Local", "link" => "/par/par.php?modulo=principal/equipeLocal&acao=A"));
	array_push($abas, array("id" => 7, "descricao" => "Comitê Local", "link" => "/par/par.php?modulo=principal/comiteLocal&acao=A"));
	
	array_push($abas, array("id" => 6, "descricao" => "Alimentação Escolar", "link" => "/par/par.php?modulo=principal/alimentacaoEscolar&acao=A"));


//	$abas = array(
//			0 => array("id" => 0, "descricao" => " {$stIntrumento} ", "link" => "/par/par.php?modulo=principal/orgaoEducacao&acao=A"),
//			1 => array("id" => 1, "descricao" => "Sec. {$stTipoIntrumento}/Dirigente {$stTipoIntrumento} de Educação", "link" => "/par/par.php?modulo=principal/dirigenteEducacao&acao=A"),
//			2 => array("id" => 2, "descricao" => "Equipe Local", "link" => "/par/par.php?modulo=principal/equipeLocal&acao=A"),
//			3 => array("id" => 3, "descricao" => "Comitê Local", "link" => "/par/par.php?modulo=principal/comiteLocal&acao=A")
//			);

	return montarAbasArray($abas, $stPaginaAtual);
}

function pegaInuidDeSbaid( $sbaid ){

	global $db;

	$sql = "SELECT iu.inuid FROM par.subacao s
			INNER JOIN par.acao a ON a.aciid = s.aciid
			INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
			INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
			WHERE s.sbaid IN ( $sbaid )";
	return $db->pegaUm( $sql );
}


function carregaAbasPropostaSubacao($stPaginaAtual = null, $frmid, $ppsid = null, $indid = null, $ptsid = null){

	$param = ($ppsid) ? "&acaoGuia=editar&ppsid=".$ppsid : "&acaoGuia=incluir&indid=".$indid;
	$abaDados = "par.php?modulo=principal/configuracao/popupGuiaSubacao&acao=A{$param}";

	if($ppsid || (strpos($stPaginaAtual, 'popupGuiaSubacao&acao=A') == 0)){
		$abaparecer = "par.php?modulo=principal/configuracao/popupGuiaSubacaoParecer&acao=A&acaoGuia=editar&ppsid={$ppsid}";
		//$abaMotivos = "par.php?modulo=principal/configuracao/popupGuiaSubacaoMotivo&acao=A&acaoGuia=editar&ppsid={$ppsid}";
		$abaitensComp = "par.php?modulo=principal/configuracao/popupGuiaSubacaoItensComp&acao=A&acaoGuia=editar&ppsid={$ppsid}";
		$abaBeneficiarios = "par.php?modulo=principal/configuracao/popupGuiaSubacaoBeneficiarios&acao=A&acaoGuia=editar&ppsid={$ppsid}";
		$abaItemComposicao = "par.php?modulo=principal/configuracao/popupGuiaSubacaoItemComposicao&acao=A{$param}";
		$abaObras = "par.php?modulo=principal/configuracao/popupGuiaSubacaoObras&acao=A{$param}";
	} else {
		$abaparecer = "javascript:void(0);";
		$abaMotivos = "javascript:void(0);";
		$abaitensComp = "javascript:void(0);";
		$abaBeneficiarios = "javascript:void(0);";
		$abaItemComposicao = "javascript:void(0);";
		$abaObras = "javascript:void(0);";
	}

	$abas = array(
			0 => array("id" => 0, "descricao" => "Dados", "link" => $abaDados),
			1 => array("id" => 1, "descricao" => "Pareceres", "link" => $abaparecer)
			);

	if($frmid == FORMA_EXECUCAO_ASSITENCIA_TECNICA && $ppsid){
		//array_push($abas, array("id" => 2, "descricao" => "Motivos", "link" => $abaMotivos));
	}elseif($frmid == FORMA_EXECUCAO_TRANSFERENCIA_VOLUNTARIA && $ppsid){
		//array_push($abas, array("id" => 3, "descricao" => "Itens de composição", "link" => $abaitensComp));
		//array_push($abas, array("id" => 4, "descricao" => "Beneficiários", "link" => $abaBeneficiarios));
	}elseif($frmid == ASSISTENCIA_FINANCEIRA_EXTRAORDINARIA || $frmid == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA && $ppsid){
		if($ptsid == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS_CONSTRUCAO_ESCOLAS || $ptsid == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS_AMPLIACAO_ESCOLAS || $ptsid == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS_REFORMA_ESCOLAS){
			array_push($abas, array("id" => 3, "descricao" => "Obras", "link" => $abaObras));
		}else{
			array_push($abas, array("id" => 2, "descricao" => "Item composição", "link" => $abaItemComposicao));
		}
	}elseif(($frmid == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO || $frmid == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO) && $ppsid){
		if($ptsid == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO_COM_ITENS || $ptsid == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO_COM_ITENS){
			array_push($abas, array("id" => 2, "descricao" => "Item composição", "link" => $abaItemComposicao));
		}
	}elseif($frmid == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS && $ppsid){
		array_push($abas, array("id" => 3, "descricao" => "Obras", "link" => $abaObras));
	}elseif($frmid == FORMA_EXECUCAO_FINANCIAMENTO_BNDES && $ppsid){
		array_push($abas, array("id" => 2, "descricao" => "Item composição", "link" => $abaItemComposicao));
	}elseif($frmid == ASSISTENCIA_FINANCEIRA_EMENDA ){
		array_push($abas, array("id" => 2, "descricao" => "Item composição", "link" => $abaItemComposicao));
	}elseif($frmid == ASSISTENCIA_FINANCEIRA_MOB_EQUIPE_PROINFANCIA && $ppsid ){
		array_push($abas, array("id" => 2, "descricao" => "Item composição", "link" => $abaItemComposicao));
	}elseif($frmid == ASSISTENCIA_FINANCEIRA_BRASIL_PRO && ($ptsid == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_BRASIL_PRO_COM_ITENS || $ptsid == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_BRASIL_PRO_FORMACAO) ){
// 	}elseif($frmid == ASSISTENCIA_FINANCEIRA_BRASIL_PRO && $ptsid == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_BRASIL_PRO_COM_ITENS ){
		array_push($abas, array("id" => 2, "descricao" => "Item composição", "link" => $abaItemComposicao));
	}elseif($frmid == ASSISTENCIA_FINANCEIRA_EMENDA_OBRAS){
		array_push($abas, array("id" => 3, "descricao" => "Obras", "link" => $abaObras));
	}elseif($frmid == ASSISTENCIA_FINANCEIRA_BRASIL_PRO_OBRAS){
		array_push($abas, array("id" => 3, "descricao" => "Obras", "link" => $abaObras));
	}

	return montarAbasArray($abas, $stPaginaAtual);
}

function carregaAbasAnosSubacao($stPaginaAtual = null){

	$abas = array(
			0 => array("id" => 0, "descricao" => "2011", "link" => "par.php?modulo=principal/popupSubacao&acao=A&anoref=2011"),
			1 => array("id" => 1, "descricao" => "2012", "link" => "par.php?modulo=principal/popupSubacao&acao=A&anoref=2012"),
			2 => array("id" => 2, "descricao" => "2013", "link" => "par.php?modulo=principal/popupSubacao&acao=A&anoref=2013"),
			3 => array("id" => 3, "descricao" => "2014", "link" => "par.php?modulo=principal/popupSubacao&acao=A&anoref=2014")
			);

	return montarAbasArray($abas, $stPaginaAtual);
}

function carregaAbasItensComposicao($stPaginaAtual = null, $icoid, $descItem = null){

	$oSubacaoControle = new SubacaoControle();
	$boTipoObra = $oSubacaoControle->verificaTipoObra($icoid);

	$boPlanilhaOrcamentaria = $oSubacaoControle->verificaPlanilhaOrcamentaria($icoid);

	if($boTipoObra){
		$lnkPlanilha   = "par.php?modulo=principal/popupItensComposicao&acao=A&tipoAba=planilhaOrcamentaria&icoid=".$icoid;
		$lnkCronograma =  "javascript:alert(\'Salve os dados da planilha orçamentária primeiro.\')";
		$lnkDocumento   = "par.php?modulo=principal/popupItensComposicao&acao=A&tipoAba=documento&icoid=".$icoid;
		$lnkFotos =  "par.php?modulo=principal/popupItensComposicao&acao=A&tipoAba=foto&icoid=".$icoid;

		if($boPlanilhaOrcamentaria){
			$lnkCronograma = "par.php?modulo=principal/popupItensComposicao&acao=A&tipoAba=cronograma&icoid=".$icoid;
		}
	}else{
		$lnkPlanilha   = "javascript:alert(\'Salve os dados do item primeiro.\')";
		$lnkCronograma = "javascript:alert(\'Salve os dados do item primeiro.\')";
		$lnkDocumento = "javascript:alert(\'Salve os dados do item primeiro.\')";
		$lnkFotos = "javascript:alert(\'Salve os dados do item primeiro.\')";
	}

	$abas = array(
			0 => array("descricao" => "Dados do $descItem", "link" => "par.php?modulo=principal/popupItensComposicao&acao=A&tipoAba=dados&icoid=".$icoid),
			1 => array("descricao" => "Documento do $descItem", "link" => $lnkDocumento),
			2 => array("descricao" => "Planilha orçamentária $descItem", "link" => $lnkPlanilha),
			3 => array("descricao" => "Cronograma Físico-Financeiro do $descItem", "link" => $lnkCronograma),
			4 => array("descricao" => "Foto do $descItem", "link" => $lnkFotos)
			);

	return montarAbasArray($abas, $stPaginaAtual);
}

function carregaAbasPronatec($stPaginaAtual = null)
{
	$preid = $_SESSION['par']['preid'] ? '&preid='.$_SESSION['par']['preid'] : '';
	$abas = array(
			0 => array("descricao" => "Termo de Compromisso",
					   "link" => "par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba=TermoCompromisso".$preid),
			1 => array("descricao" => "Dados do Imóvel",
					   "link" => "par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba=Dados".$preid)
			);
	if($_SESSION['par']['preid']){
		array_push($abas, array("descricao" => "Características do Imóvel",
					   			"link" => "par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba=Questionario".$preid));
		array_push($abas, array("descricao" => "Cadastro de Fotos do Imóvel",
					   			"link" => "par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba=Foto".$preid));
		array_push($abas, array("descricao" => "Documentos Anexos",
					   			"link" => "par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba=Documento".$preid));
		array_push($abas, array("descricao" => "Analise",
					   			"link" => "par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba=Analise".$preid));
		array_push($abas, array("descricao" => "Analise Instituto Federal",
					   			"link" => "par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba=AnaliseEngenheiro".$preid));
	}

	$win = false;

	return montarAbasArray($abas, $stPaginaAtual, $win);
}

// Verifico se a Obra do PAC é de MI
function verificaMi( $preid = null ){
	global $db;

	if( !$preid ){
		return false;
	}

	$verifica = false;
	$categoria = $db->pegaUm("SELECT
							pto.ptocategoria
						FROM
							obras.preobra po
						INNER JOIN obras.pretipoobra pto ON pto.ptoid = po.ptoid
						WHERE
							po.preid = ".$preid );

	if( $categoria ){
		$verifica = true;
	}
	return $verifica;
}

// Verifico se a Obra do PAC é de MI
function verificaMiConvencional( $preid = null ){
	global $db;

	if( !$preid ){
		return false;
	}

	$verifica = false;

	$sql = "SELECT
				TRUE
			FROM
				obras.preobra pre
			INNER JOIN workflow.documento 		doc ON doc.docid = pre.docid
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			WHERE
				pre.preid = 130030
				AND esd.esddsc ilike '%RMC%'";

	$verifica = $db->pegaUm($sql);

	return $verifica == 't';
}


// Verifico se a Obra do PAR é de Emenda
function verificaEmenda( $preid = null ){
	global $db;

	if( !$preid ){
		return false;
	}

	$verifica = false;
	$tipo = $db->pegaUm("SELECT
									pto.ptoid
								FROM
									obras.preobra pre
								INNER JOIN obras.pretipoobra pto on pto.ptoid = pre.ptoid AND pto.ptodescricao ilike '%emenda%'
								WHERE
									pre.preid = ".$preid );

	if( $tipo ){
		$verifica = true;
	}
	return $verifica;
}

function carregaAbasProInfancia($stPaginaAtual = null, $preid = null, $descItem = null){

	global $db;

	$oSubacaoControle = new SubacaoControle();

	$sql = "SELECT ptoid FROM obras.preobra WHERE preid = $preid";
	$ptoid = $db->pegaUm($sql);

	if($preid && $ptoid){

		$docid = prePegarDocid($preid);
		$esdid = prePegarEstadoAtual($docid);
		//$isReformulacao = $db->pegaUm("SELECT preidpai FROM obras.preobra WHERE preid='".$preid."'");
		$isReformulacao = $db->pegaUm("SELECT esd.esdid FROM workflow.documento d
										INNER JOIN obras.preobra p ON p.docid = d.docid
										INNER JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
										WHERE p.preid='".$preid."'");

		$lnkPendencias	= "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=analise&preid=".$preid;

		$reformulaMI = verificaMi( $preid );

		$boTipoObra 			= $oSubacaoControle->verificaTipoObra($preid, SIS_OBRAS);
		$boPlanilhaOrcamentaria = $oSubacaoControle->verificaPlanilhaOrcamentaria(null, SIS_OBRAS, $preid);
		$tipoObra 				= $oSubacaoControle->verificaTipoObra($preid, SIS_OBRAS);
		$pacFNDE 				= $oSubacaoControle->verificaObraFNDE($preid, SIS_OBRAS);

		$lnkDocumento   	= "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=documento&preid=".$preid;
		$lnkDocumentoFNDE   = "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=documentoFNDE&preid=".$preid;
//		$lnkDocumentoTipoA  = "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=documentoTipoA&preid=".$preid;

		if($boTipoObra){

			$lnkPlanilha	= "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=planilhaOrcamentaria&preid=".$preid;
			$lnkCronograma 	= "javascript:alert(\'Salve os dados da planilha orçamentária primeiro.\')";
			$lnkFotos 		= "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=foto&preid=".$preid;

			if($boPlanilhaOrcamentaria){
				$lnkCronograma  = "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=cronograma&preid=".$preid;
				$lnkPendencias	= "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=analise&preid=".$preid;
			}

		}else{
			$lnkPlanilha	= "javascript:alert(\'Informe o tipo de obra e o endereço na aba de dados do terreno.\')";
			$lnkCronograma 	= "javascript:alert(\'Informe o tipo de obra e o endereço na aba de dados do terreno.\')";
			$lnkFotos 		= "javascript:alert(\'Informe o tipo de obra e o endereço na aba de dados do terreno.\')";
		}

		$lnkDados 			= "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=dados&preid=".$preid;
		$lnkOrcamentarios 	= "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=dadosOrcamentarios&preid=".$preid;
		$lnkQuestionario 	= "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=questionario&preid=".$preid;

	}elseif($preid){

		$lnkPlanilha		= "javascript:alert('Salve o tipo de obra primeiro.')";
		$lnkCronograma 		= "javascript:alert('Salve o tipo de obra primeiro.')";
		$lnkDocumento 		= "javascript:alert('Salve o tipo de obra primeiro.')";
		$lnkFotos 			= "javascript:alert('Salve o tipo de obra primeiro.')";
		$lnkDados 			= "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=dados&preid=".$preid;
		$lnkOrcamentarios 	= "javascript:alert('Salve o tipo de obra primeiro.')";
		$lnkQuestionario 	= "javascript:alert('Salve o tipo de obra primeiro.')";
		$lnkPendencias 		= "javascript:alert('Salve o tipo de obra primeiro.')";
		$lnkDocumentoFNDE 	= "javascript:alert('Salve o tipo de obra primeiro.')";

	}else{

		$lnkPlanilha		= "javascript:alert(\'Salve os dados do terreno primeiro.\')";
		$lnkCronograma 		= "javascript:alert(\'Salve os dados do terreno primeiro.\')";
		$lnkDocumento 		= "javascript:alert(\'Salve os dados do terreno primeiro.\')";
//		$lnkDocumentoTipoA	= "javascript:alert(\'Salve os dados do terreno primeiro.\')";
		$lnkFotos 			= "javascript:alert(\'Salve os dados do terreno primeiro.\')";
		$lnkDados 			= "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A";
		$lnkOrcamentarios 	= "javascript:alert(\'Salve os dados do terreno primeiro.\')";
		$lnkQuestionario 	= "javascript:alert(\'Salve os dados do terreno primeiro.\')";

	}

	$abas = array(
			0 => array("descricao" => "Dados do terreno", "link" => $lnkDados)
			);

	if($preid){
		array_push($abas, array("descricao" => "Relatório de vistoria", "link" => $lnkQuestionario));
		array_push($abas, array("descricao" => "Cadastro de fotos do terreno", "link" => $lnkFotos));
	}

	if($preid){

		if($pacFNDE == 't'){
			array_push($abas, array("descricao" => "Planilha orçamentária", "link" => $lnkPlanilha));
			if( !($reformulaMI) && !in_array($ptoid, Array(73,74)) ){
				array_push($abas, array("descricao" => "Cronograma Físico-Financeiro", "link" => $lnkCronograma));
			}
		}else if( $pacFNDE == 'f' && possuiPerfil( Array(PAR_PERFIL_COORDENADOR_GERAL,
														PAR_PERFIL_ENGENHEIRO_FNDE,
														PAR_PERFIL_EQUIPE_MUNICIPAL,
														PAR_PERFIL_PREFEITO,
														PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,
														PAR_PERFIL_EQUIPE_ESTADUAL,
														PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO) ) ){
			array_push($abas, array("descricao" => "Planilha orçamentária", "link" => $lnkPlanilha));
		}

		$lnkRecursoEmendas = "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&preid=$preid&tipoAba=CronogramaContrapartida";
		array_push($abas, array("descricao" => "Cronograma de Contrapartida", "link" => $lnkRecursoEmendas));

		array_push($abas, array("descricao" => "Documentos anexos", "link" => $lnkDocumento));

//		if($pacFNDE == 'f'){
//			array_push($abas, array("descricao" => "Projetos - Tipo A", "link" => $lnkDocumentoTipoA));
//		}

		$obraMiParaConvencialDistrato = false;
		$obraMiParaConvencialDistrato = testaObraMIDestrato( $preid );
		$arrEstado = Array(WF_TIPO_EM_CADASTRAMENTO,WF_TIPO_EM_CORRECAO);
		$arrPerfilsAnalise = array(PAR_PERFIL_EQUIPE_MUNICIPAL,PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, PAR_PERFIL_PREFEITO, PAR_PERFIL_ENGENHEIRO_FNDE, PAR_PERFIL_COORDENADOR_GERAL, PAR_PERFIL_COORDENADOR_TECNICO, PAR_PERFIL_SUPER_USUARIO, PAR_PERFIL_ADMINISTRADOR);

		if( possuiPerfil( Array(PAR_PERFIL_SUPER_USUARIO, PAR_PERFIL_ADMINISTRADOR) ) ){
			array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias));
		} else{
			// se não for uma reformulação
			if( in_array($isReformulacao, Array(WF_TIPO_EM_REFORMULACAO, WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL, WF_TIPO_EM_DILIGENCIA_REFORMULACAO_MI_PARA_CONVENCIONAL) ) ) {
				if( possuiPerfil( Array(PAR_PERFIL_CONSULTA) ) ){
					array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias));
				}elseif( possuiPerfil(PAR_PERFIL_EQUIPE_MUNICIPAL) || possuiPerfil(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO) || possuiPerfil(PAR_PERFIL_PREFEITO) || possuiPerfil(PAR_PERFIL_EQUIPE_ESTADUAL) || possuiPerfil(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO) ){
					if(  in_array( $esdid, Array(WF_PAR_OBRA_VAL_INDEF_REFORMULACAO, WF_TIPO_EM_REFORMULACAO, WF_TIPO_EM_CADASTRAMENTO, WF_TIPO_EM_CORRECAO, WF_TIPO_OBRA_ARQUIVADA, WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL, WF_TIPO_EM_DILIGENCIA_REFORMULACAO_MI_PARA_CONVENCIONAL) ) ){
						array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias));
					}
				}elseif( possuiPerfil(PAR_PERFIL_ENGENHEIRO_FNDE) ){
					if(  !in_array( $esdid, Array(WF_TIPO_OBRA_INDEFERIDA, WF_TIPO_OBRA_DEFERIDA, WF_TIPO_OBRA_CONDICIONADA, WF_TIPO_OBRA_INDEFERIDA_PRAZO, WF_TIPO_OBRA_APROVADA, WF_TIPO_OBRA_ARQUIVADA) ) ){
						array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias));
					}
				}elseif( possuiPerfil(PAR_PERFIL_COORDENADOR_GERAL) ){
					if(  !in_array( $esdid, Array(WF_TIPO_OBRA_INDEFERIDA, WF_TIPO_OBRA_DEFERIDA, WF_TIPO_OBRA_CONDICIONADA, WF_TIPO_OBRA_INDEFERIDA_PRAZO, WF_TIPO_OBRA_APROVADA, WF_TIPO_OBRA_ARQUIVADA) ) ){
						array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias));
					}
				}
			}elseif( possuiPerfil(PAR_PERFIL_EQUIPE_MUNICIPAL) || possuiPerfil(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO) || possuiPerfil(PAR_PERFIL_PREFEITO) || possuiPerfil(PAR_PERFIL_EQUIPE_ESTADUAL) || possuiPerfil(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO) ){

				if(  in_array( $esdid, Array(WF_PAR_OBRA_VAL_INDEF_REFORMULACAO, WF_TIPO_EM_CADASTRAMENTO, WF_TIPO_EM_CORRECAO, WF_TIPO_OBRA_ARQUIVADA,WF_TIPO_EM_REFORMULACAO) ) || ($obraMiParaConvencialDistrato) ){
					array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias));
				}
			}elseif( possuiPerfil(array(PAR_PERFIL_COORDENADOR_GERAL, PAR_PERFIL_ENGENHEIRO_FNDE))){
				if( $reformulaMI == true || in_array( $esdid, Array(WF_TIPO_OBRA_AGUARDANDO_PRORROGACAO, WF_TIPO_EM_VALIDACAO_DEFERIMENTO_REFORMULACAO) ) ){
					array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias));
				}
			}

		}


//		if( in_array($esdid,$arrEstado) && possuiPerfil($arrPerfilsAnalise)){
//
//			if(verificaWFpreObra( $preid ) == false){
//				array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias));
//			}
//			elseif($db->testa_superuser()){
//			array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias));}
//
//
//		}elseif($db->testa_superuser()){
//			array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias));
//		}
	}

	$perfil = pegaArrayPerfil($_SESSION['usucpf']);
	/*
	$arSituacoes = array(WF_TIPO_EM_ANALISE_FNDE,
						 WF_TIPO_EM_ANALISE_REFORMULACAO,
						 WF_TIPO_EM_VALIDACAO_DEFERIMENTO_REFORMULACAO,
						 WF_TIPO_EM_VALIDACAO_INDEFERIMENTO_REFORMULACAO,
						 WF_TIPO_EM_ANALISE,
						 WF_TIPO_VALIDACAO_DILIGENCIA,
						 WF_TIPO_VALIDACAO_INDEFERIMENTO,
						 WF_TIPO_VALIDACAO_DEFERIMENTO,
						 WF_TIPO_OBRA_INDEFERIDA,
						 WF_TIPO_OBRA_DEFERIDA,
						 WF_TIPO_EM_ANALISE_VALIDACAO,
						 WF_TIPO_EM_ANALISE_DILIGENCIA,
						 WF_TIPO_OBRA_CONDICIONADA,
						 WF_TIPO_OBRA_INDEFERIDA_PRAZO,
						 WF_TIPO_OBRA_APROVADA,
						 WF_TIPO_OBRA_ARQUIVADA,
						 WF_TIPO_EM_ANALISE_REFORMULACAO_MI_PARA_CONVENCIONAL);
	$arSituacoesEquipe = array(WF_TIPO_EM_CORRECAO,WF_TIPO_OBRA_ARQUIVADA);*/
	// Mostra aba se o estado for em Análise - FNDE
	/*
	if( in_array($esdid, $arSituacoes) && (
            in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ||
            in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ||
            in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
            in_array(PAR_PERFIL_CONSULTA, $perfil) ||
            in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) ||
            ( $esdid == WF_TIPO_EM_ANALISE_REFORMULACAO && $perfil == PAR_PERFIL_ENGENHEIRO_FNDE ))
            || in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $perfil)
        ) {

			$lnkAnaliseEngenheiro	= "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=analiseEngenheiro&preid=".$preid;

			array_push($abas, array("descricao" => "Análise de Engenharia", "link" => $lnkAnaliseEngenheiro));

	}elseif(in_array($esdid, $arSituacoesEquipe) && (in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ||
            in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ||
			in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
			in_array(PAR_PERFIL_CONSULTA, $perfil) ||
			in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) ||
			( $esdid == WF_TIPO_EM_ANALISE_REFORMULACAO && in_array(PAR_PERFIL_ENGENHEIRO_FNDE,$perfil) ) ||
			possuiPerfil(array(PAR_PERFIL_EQUIPE_MUNICIPAL,PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,PAR_PERFIL_EQUIPE_ESTADUAL,PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, PAR_PERFIL_PREFEITO))
			//in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, PAR_PERFIL_PREFEITO, $perfil)
			)){

			$lnkAnaliseEngenheiro	= "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=analiseEngenheiro&preid=".$preid;

			array_push($abas, array("descricao" => "Análise de Engenharia", "link" => $lnkAnaliseEngenheiro));

	} elseif($isReformulacao == WF_TIPO_EM_REFORMULACAO) {
		$lnkAnaliseEngenheiro	= "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=analiseEngenheiro&preid=".$preid;

		array_push($abas, array("descricao" => "Análise de Engenharia", "link" => $lnkAnaliseEngenheiro));

	} elseif( $esdid == WF_TIPO_OBRA_APROVACAO_CONDICIONAL &&
			  (in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) ||
		      in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
		      in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ||
			  in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) )   ) {
		$lnkAnaliseEngenheiro	= "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=analiseEngenheiro&preid=".$preid;

		array_push($abas, array("descricao" => "Análise de Engenharia", "link" => $lnkAnaliseEngenheiro));
	}
	*/
	/* Nova regra para Análise de Engenharia - 11/06/2015
	 * 	- Mostra que não estiver em cadastramento ou reformulação.
	 * Alterado por Thiago e Eduardo*/
	/*
	 * A pedido do Eduardo Nunes, mostra em reformulação
	 * Demanda PAR 1967, 28/07/2015
	 * */

	$arrEsdid = Array(
			WF_TIPO_EM_CADASTRAMENTO//,
// 			WF_TIPO_EM_REFORMULACAO,
// 			WF_TIPO_EM_REFORMULACAO_OBRAS_MI,
// 			WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL
		);
	if( !in_array($esdid, $arrEsdid ) ){

		$lnkAnaliseEngenheiro	= "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=analiseEngenheiro&preid=".$preid;

		array_push($abas, array("descricao" => "Análise de Engenharia", "link" => $lnkAnaliseEngenheiro));
	}

	if($preid && possuiPerfil(array(PAR_PERFIL_ADMINISTRADOR,PAR_PERFIL_ENGENHEIRO_FNDE))){

		$lnkListaObras	= "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=listaObras&preid=".$preid;
		$titulo = $_SESSION['par']['esfera'] == 'M' ? "Obras no Município" : "Obras no Estado";
		array_push($abas, array("descricao" => $titulo, "link" => $lnkListaObras));
	}

	$win = false;

	if((in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ||
        in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ||
	    in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
	    in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) ||
	    ( $esdid == WF_TIPO_EM_ANALISE_REFORMULACAO && in_array(PAR_PERFIL_ENGENHEIRO_FNDE,$perfil) ) ||
	    in_array(PAR_PERFIL_COORDENADOR_TECNICO, $perfil)) && ( $_GET['tipoAba'] == 'analiseEngenheiro' )){
	   	$win = true;
	}

	// Adiciona a aba de empenho
//	$lnkEmpenho = "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=empenho&preid=".$preid;
//	array_push($abas, array("descricao" => "Empenho", "link" => $lnkEmpenho));
//	$lnkPagamento = "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=pagamento&preid=".$preid;
//	array_push($abas, array("descricao" => "Pagamento", "link" => $lnkPagamento));

	if( $preid && (in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ||
         in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ||
		 in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
		 in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) ||
		 ( $esdid == WF_TIPO_EM_ANALISE_REFORMULACAO && in_array(PAR_PERFIL_ENGENHEIRO_FNDE,$perfil) ) ) ) {
		 array_push($abas, array("descricao" => "Documentos FNDE", "link" => $lnkDocumentoFNDE));
	}


	if($preid){
		array_push($abas, array("descricao" => "Dados Orçamentários", "link" => $lnkOrcamentarios));
	}

	return montarAbasArray($abas, $stPaginaAtual, $win);
}

function carregaAbasSubacaoObras($stPaginaAtual = null, $param = Array() ){

	global $db;

	$preid = $param['preid'];
	$sbaid = $param['sbaid'];
	$ano   = $param['ano'];
	$perfil = pegaArrayPerfil($_SESSION['usucpf']);

	$anoCorrente = date('Y');

	$oSubacaoControle = new SubacaoControle();

	if( $preid ){
		$sql = "SELECT ptoid FROM obras.preobra WHERE preid = $preid";
		$ptoid = $db->pegaUm($sql);
	}

	if($preid && $ptoid){

		$docid = prePegarDocid($preid);
		$esdid = prePegarEstadoAtual($docid);
		//$isReformulacao = $db->pegaUm("SELECT preidpai FROM obras.preobra WHERE preid='".$preid."'");
		$isReformulacao = $db->pegaUm("SELECT esd.esdid FROM workflow.documento d
									INNER JOIN obras.preobra p ON p.docid = d.docid
									INNER JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
									WHERE p.preid='".$preid."'");

		$lnkPendencias	= "par.php?modulo=principal/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid&aba=Analise";

		$boTipoObra 			= $oSubacaoControle->verificaTipoObra($preid, SIS_OBRAS);
		$boPlanilhaOrcamentaria = $oSubacaoControle->verificaPlanilhaOrcamentaria(null, SIS_OBRAS, $preid);
		$tipoObra 				= $oSubacaoControle->verificaTipoObra($preid, SIS_OBRAS);
		$pacFNDE 				= $oSubacaoControle->verificaObraFNDE($preid, SIS_OBRAS);

		$lnkDocumento   	= "par.php?modulo=principal/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid&aba=Documento";
//		$lnkDocumentoTipoA  = "par.php?modulo=principal/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid&aba=DocumentoTipoA";

		if($boTipoObra){

			$lnkPlanilha	= "par.php?modulo=principal/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid&aba=PlanilhaOrcamentaria";
			$lnkCronograma 	= "javascript:alert(\'Salve os dados da planilha orçamentária primeiro.\')";
			$lnkFotos 		= "par.php?modulo=principal/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid&aba=Fotos";

			if($boPlanilhaOrcamentaria){

				$lnkCronograma  = "par.php?modulo=principal/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid&aba=CronogramaFisicoFinanceiro";
				$lnkPendencias	= "par.php?modulo=principal/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid&aba=Analise";
			}

		}else{
			$lnkPlanilha	= "javascript:alert(\'Informe o tipo de obra e o endereço na aba de dados do terreno.\')";
			$lnkCronograma 	= "javascript:alert(\'Informe o tipo de obra e o endereço na aba de dados do terreno.\')";
			$lnkFotos 		= "javascript:alert(\'Informe o tipo de obra e o endereço na aba de dados do terreno.\')";
		}

		$lnkDados 				= "par.php?modulo=principal/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid";
		$lnkDadosOrcamentarios 	= "par.php?modulo=principal/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid&aba=DadosOrcamentarios";
		$lnkQuestionario 		= "par.php?modulo=principal/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid&aba=Questionario";
		$lnkDocumentosFNDE		= "par.php?modulo=principal/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid&aba=DocumentoFNDE";

	}elseif($preid){

		$lnkPlanilha			= "javascript:alert('Salve o tipo de obra primeiro.')";
		$lnkCronograma 			= "javascript:alert('Salve o tipo de obra primeiro.')";
		$lnkDocumento 			= "javascript:alert('Salve o tipo de obra primeiro.')";
		$lnkFotos 				= "javascript:alert('Salve o tipo de obra primeiro.')";
		$lnkDados 				= "par.php?modulo=principal/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid";
		$lnkQuestionario 		= "javascript:alert('Salve o tipo de obra primeiro.')";
		$lnkDadosOrcamentarios 	= "javascript:alert('Salve o tipo de obra primeiro.')";
		$lnkDocumentosFNDE		= "javascript:alert('Salve o tipo de obra primeiro.')";
		$lnkPendencias			= "javascript:alert('Salve o tipo de obra primeiro.')";

	}else{

		$lnkPlanilha			= "javascript:alert('Salve os dados do terreno primeiro.')";
		$lnkCronograma 			= "javascript:alert('Salve os dados do terreno primeiro.')";
		$lnkDocumento 			= "javascript:alert('Salve os dados do terreno primeiro.')";
		$lnkFotos 				= "javascript:alert('Salve os dados do terreno primeiro.')";
		$lnkDados 				= "par.php?modulo=principal/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano";
		$lnkQuestionario 		= "javascript:alert('Salve os dados do terreno primeiro.')";
		$lnkDadosOrcamentarios 	= "javascript:alert('Salve os dados do terreno primeiro.')";
		$lnkDocumentosFNDE		= "javascript:alert('Salve os dados do terreno primeiro.')";
	}

	$abas = array(0 => array("descricao" => "Dados do terreno", "link" => $lnkDados));

//	Retirei a regra pois foi informado pelo Cabral no dia 07/02/2012 que não existiria mais essa regra.
//  Recoloquei a regra pois foi informado pelo Daniel no dia 23/03/2012 que era pra regra voltar a existir.

	#Regra modificada na data de 09/08/2012: Essa regra foi solicitado pelo analista Thiago e a ele foi solicitada por Romeu.
	#Indepedente do ano ser maior ou menor que o ano corrente ira montar as abas (todas).
	#Anterior: $ano == $anoCorrente || $ano < $anoCorrente

	if( $ano == $anoCorrente || $ano <> $anoCorrente){
		if($preid){
			array_push($abas, array("descricao" => "Relatório de vistoria", 		"link" => $lnkQuestionario));
			array_push($abas, array("descricao" => "Cadastro de fotos do terreno", 	"link" => $lnkFotos));
		}

		if($preid){

			if($pacFNDE == 't'){
				array_push($abas, array("descricao" => "Planilha orçamentária", "link" => $lnkPlanilha));
				if( !in_array($ptoid, Array(73,74)) ){
					array_push($abas, array("descricao" => "Cronograma Físico-Financeiros", "link" => $lnkCronograma));
				}
			}elseif( $pacFNDE == 'f' &&
					 possuiPerfil( Array(PAR_PERFIL_COORDENADOR_GERAL) ) ||
					 ( $esdid == WF_PAR_EM_REVISAO_DE_ANALISE && in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ) ||
					 ( $esdid == WF_PAR_EM_CADASTRAMENTO ) ||
					 ( $esdid == WF_PAR_OBRA_EM_CADASTRAMENTO_CONDICIONAL ) ||
					 ( $esdid == WF_PAR_EM_ANALISE_RETORNO_DA_DILIGENCIA ) ||
					 ( $esdid == WF_PAR_EM_ANALISE_FNDE ) ||
					 (
					 	in_array( $esdid, Array( WF_PAR_EM_DILIGENCIA, WF_PAR_OBRA_EM_REFORMULACAO ) ) &&
					 	possuiPerfil( Array(PAR_PERFIL_PREFEITO, PAR_PERFIL_EQUIPE_MUNICIPAL, PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,
					 						PAR_PERFIL_EQUIPE_ESTADUAL, PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO,
					 						PAR_PERFIL_SUPER_USUARIO, PAR_PERFIL_ADMINISTRADOR) )
					 )
					){
				array_push($abas, array("descricao" => "Planilha orçamentária", "link" => $lnkPlanilha));
			}

			$frmid = $db->pegaUm("select frmid from par.subacao where sbaid = $sbaid");

			if( in_array($frmid, array(14, 15)) ){
				$lnkRecursoEmendas = "par.php?modulo=principal/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid&aba=RecursoEmendas";
				array_push($abas, array("descricao" => "Recursos de Emendas", "link" => $lnkRecursoEmendas));
			}

			array_push($abas, array("descricao" => "Documentos anexos", "link" => $lnkDocumento));

	//		if($pacFNDE == 'f'){
	//			array_push($abas, array("descricao" => "Projetos - Tipo A", "link" => $lnkDocumentoTipoA));
	//		}

			$arrEstado = Array(WF_PAR_EM_CADASTRAMENTO, WF_PAR_EM_REVISAO_DE_ANALISE);
			$arrPerfilsAnalise = array(PAR_PERFIL_EQUIPE_MUNICIPAL,PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, PAR_PERFIL_EQUIPE_ESTADUAL, PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, PAR_PERFIL_PREFEITO, PAR_PERFIL_ENGENHEIRO_FNDE, PAR_PERFIL_COORDENADOR_GERAL, PAR_PERFIL_COORDENADOR_TECNICO, PAR_PERFIL_SUPER_USUARIO, PAR_PERFIL_ADMINISTRADOR);

	// se não for uma reformulação
			/*if(!$isReformulacao) {
				if( possuiPerfil( Array(PAR_PERFIL_SUPER_USUARIO,PAR_PERFIL_CONSULTA) ) ){
					array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias));
				}elseif( possuiPerfil(PAR_PERFIL_EQUIPE_MUNICIPAL) || possuiPerfil(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO) || possuiPerfil(PAR_PERFIL_PREFEITO) || possuiPerfil(PAR_PERFIL_EQUIPE_ESTADUAL) || possuiPerfil(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO) ){
					if(  in_array( $esdid, Array(WF_PAR_EM_CADASTRAMENTO, WF_PAR_EM_REVISAO_DE_ANALISE, WF_PAR_OBRA_ARQUIVADA, WF_PAR_EM_DILIGENCIA) ) ){
						array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias));
					}
				}elseif( possuiPerfil(PAR_PERFIL_ENGENHEIRO_FNDE) ){
					if(  !in_array( $esdid, Array(WF_PAR_OBRA_INDEFERIDA, WF_PAR_OBRA_DEFERIDA, WF_PAR_OBRA_CONDICIONADA, WF_PAR_OBRA_INDEFERIDA_PRAZO, WF_PAR_OBRA_APROVADA, WF_PAR_OBRA_ARQUIVADA) ) ){
						array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias));
					}
				}elseif( possuiPerfil(PAR_PERFIL_COORDENADOR_GERAL) ){
					if(  !in_array( $esdid, Array(WF_PAR_OBRA_INDEFERIDA, WF_PAR_OBRA_DEFERIDA, WF_PAR_OBRA_CONDICIONADA, WF_PAR_OBRA_INDEFERIDA_PRAZO, WF_PAR_OBRA_APROVADA, WF_PAR_OBRA_ARQUIVADA) ) ){
						array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias));
					}
				}
			}*/
			if( possuiPerfil( Array(PAR_PERFIL_SUPER_USUARIO, PAR_PERFIL_ADMINISTRADOR) ) ){
				array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias));
			} else{
				$arrEsdid = Array(WF_PAR_OBRA_EM_REFORMULACAO, WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL_PAR, WF_TIPO_EM_DILIGENCIA_REFORMULACAO_MI_PARA_CONVENCIONAL_PAR);
				if( in_array($esdid, $arrEsdid) ) {
					if( possuiPerfil( Array(PAR_PERFIL_CONSULTA) ) ){
						array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias));
					}elseif( possuiPerfil(PAR_PERFIL_EQUIPE_MUNICIPAL) || possuiPerfil(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO) || possuiPerfil(PAR_PERFIL_PREFEITO) || possuiPerfil(PAR_PERFIL_EQUIPE_ESTADUAL) || possuiPerfil(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO) ){
						if(  in_array( $esdid, Array(WF_PAR_OBRA_EM_VALIDACAO_INDEFERIMENTO_REFORMULACAO, WF_PAR_OBRA_EM_REFORMULACAO, WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL_PAR, WF_TIPO_EM_DILIGENCIA_REFORMULACAO_MI_PARA_CONVENCIONAL_PAR, WF_PAR_EM_CADASTRAMENTO,  WF_PAR_OBRA_ARQUIVADA) ) ){
							array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias));
						}
					}elseif( possuiPerfil(PAR_PERFIL_ENGENHEIRO_FNDE) ){
						if(  !in_array( $esdid, Array(WF_PAR_OBRA_INDEFERIDA, WF_PAR_OBRA_DEFERIDA, WF_PAR_OBRA_CONDICIONADA, WF_PAR_OBRA_INDEFERIDA_PRAZO, WF_PAR_OBRA_APROVADA, WF_PAR_OBRA_ARQUIVADA) ) ){
							array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias));
						}
					}elseif( possuiPerfil(PAR_PERFIL_COORDENADOR_GERAL) ){
						if(  !in_array( $esdid, Array(WF_PAR_OBRA_INDEFERIDA, WF_PAR_OBRA_DEFERIDA, WF_PAR_OBRA_CONDICIONADA, WF_PAR_OBRA_INDEFERIDA_PRAZO, WF_PAR_OBRA_APROVADA, WF_PAR_OBRA_ARQUIVADA) ) ){
							array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias));
						}
					}
				}elseif( possuiPerfil(PAR_PERFIL_COORDENADOR_GERAL) || possuiPerfil(PAR_PERFIL_ADMINISTRADOR) || possuiPerfil(PAR_PERFIL_EQUIPE_MUNICIPAL) || possuiPerfil(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO) || possuiPerfil(PAR_PERFIL_PREFEITO) || possuiPerfil(PAR_PERFIL_EQUIPE_ESTADUAL) || possuiPerfil(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO) ){
					if(  in_array( $esdid, Array(WF_PAR_OBRA_VAL_INDEF_REFORMULACAO, WF_PAR_EM_CADASTRAMENTO, WF_PAR_OBRA_ARQUIVADA, WF_PAR_OBRA_EM_REFORMULACAO, WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL_PAR, WF_TIPO_EM_DILIGENCIA_REFORMULACAO_MI_PARA_CONVENCIONAL_PAR, WF_PAR_EM_DILIGENCIA, WF_PAR_OBRA_EM_CADASTRAMENTO_CONDICIONAL) ) ){
						array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias));
					}
				}
			}

		}
/*
		$arSituacoes = array(WF_PAR_EM_ANALISE_FNDE,
							 WF_PAR_EM_ANALISE,
							 WF_PAR_EM_DILIGENCIA,
							 WF_PAR_VALIDACAO_DILIGENCIA,
							 WF_PAR_VALIDACAO_INDEFERIMENTO,
							 WF_PAR_VALIDACAO_DEFERIMENTO,
							 WF_PAR_OBRA_INDEFERIDA,
							 WF_PAR_OBRA_DEFERIDA,
							 WF_PAR_OBRA_CONDICIONADA,
							 WF_PAR_OBRA_INDEFERIDA_PRAZO,
							 WF_PAR_EM_ANALISE_RETORNO_DA_DILIGENCIA,
							 WF_PAR_OBRA_APROVADA,
							 WF_PAR_OBRA_ARQUIVADA);
		$arSituacoesEquipe = array(WF_PAR_EM_REVISAO_DE_ANALISE,WF_PAR_OBRA_ARQUIVADA);
*/
		// Mostra aba se o estado for em Análise - FNDE
		/*
		if( in_array($esdid, $arSituacoes) && (in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ||
                in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ||
				in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
				in_array(PAR_PERFIL_CONSULTA, $perfil) ||
				in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil))) {

				$lnkAnaliseEngenheiro	= "par.php?modulo=principal/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid&aba=AnaliseEngenheiro";

				array_push($abas, array("descricao" => "Análise de Engenharia", "link" => $lnkAnaliseEngenheiro));

		}elseif(in_array($esdid, $arSituacoesEquipe) && (in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ||
                in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ||
                in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
				in_array(PAR_PERFIL_CONSULTA, $perfil) ||
				in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) ||
				in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) ||
				in_array(PAR_PERFIL_PREFEITO, $perfil) ||
				in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil))){

				$lnkAnaliseEngenheiro	= "par.php?modulo=principal/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid&aba=AnaliseEngenheiro";

				array_push($abas, array("descricao" => "Análise de Engenharia", "link" => $lnkAnaliseEngenheiro));

		} elseif( in_array($esdid, array(WF_PAR_EM_DILIGENCIA)) && (in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) | in_array(PAR_PERFIL_PREFEITO, $perfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil)) ) {

				$lnkAnaliseEngenheiro	= "par.php?modulo=principal/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid&aba=AnaliseEngenheiro";

				array_push($abas, array("descricao" => "Análise de Engenharia", "link" => $lnkAnaliseEngenheiro));

		} elseif($isReformulacao) {
			$lnkAnaliseEngenheiro	= "par.php?modulo=principal/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid&aba=AnaliseEngenheiro";

			array_push($abas, array("descricao" => "Análise de Engenharia", "link" => $lnkAnaliseEngenheiro));

		}
		*/

		$arrEsdidCad = Array(WF_PAR_EM_CADASTRAMENTO );
		/* Alterado na Demanda PAR 1855 a pedido do Eduardo Nunes da Silva, 28/07/2015
		$arrEsdidCad = Array(WF_PAR_EM_CADASTRAMENTO, WF_PAR_OBRA_EM_REFORMULACAO, WF_PAR_OBRA_EM_REFORMULACAO_MI_CONVENCIONAL );
		 * */
		if( !in_array($esdid, $arrEsdidCad ) && $preid ){
			$lnkAnaliseEngenheiro	= "par.php?modulo=principal/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid&aba=AnaliseEngenheiro";

			array_push($abas, array("descricao" => "Análise de Engenharia", "link" => $lnkAnaliseEngenheiro));
		}

		if(possuiPerfil(array(PAR_PERFIL_ADMINISTRADOR,PAR_PERFIL_ENGENHEIRO_FNDE))){

			$lnkListaObras	= "par.php?modulo=principal/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid&aba=ListaObras";
			$titulo = $_SESSION['par']['itrid'] == 2 ? "Obras no Município" : "Obras no Estado";
			array_push($abas, array("descricao" => $titulo, "link" => $lnkListaObras));
		}

		$win = false;
		if((in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ||
            in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ||
		    in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
		    in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) ||
		    in_array(PAR_PERFIL_COORDENADOR_TECNICO, $perfil)) && ( $_GET['tipoAba'] == 'analiseEngenheiro' )){
		   	$win = true;
		}
		if($preid){
			array_push($abas, array("descricao" => "Dados Orçamentarios", 			"link" => $lnkDadosOrcamentarios));
			array_push($abas, array("descricao" => "Documentos FNDE", 				"link" => $lnkDocumentosFNDE));
		}
	}

	return montarAbasArray($abas, $stPaginaAtual, $win);
}

function carregaAbasResolucao($stPaginaAtual = null){

	global $db;

	$abas = array( 0 => array( "descricao" => "Cadastro de Resolução",
							   "link"	   => "par.php?modulo=sistema/cadastro/cadastroresolucao&acao=I" ),
		  		   1 => array( "descricao" => "Vinculo de Resolução",
							   "link"	   => "par.php?modulo=sistema/cadastro/vinculoresolucao&acao=I" )
		 			);

	return montarAbasArray($abas, $stPaginaAtual, false);
}

function pegaQrpid( $inuid, $queid, $estuf, $muncod = null ){

	global $db;

    include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";

    $sql = "SELECT
            	qp.qrpid
            FROM
            	par.parquestionario qp
            INNER JOIN questionario.questionarioresposta qr ON qr.qrpid = qp.qrpid
            WHERE
            	qp.inuid = {$inuid}
            	AND qr.queid = {$queid}";
    $qrpid = $db->pegaUm( $sql );

    if(!$qrpid){
    	$sql = "SELECT DISTINCT
    				'true'
    			FROM
            		questionario.questionario
            	WHERE
            		queid = ".$queid;
    	$testaQueid = $db->pegaUm($sql);
    	if( $testaQueid ){
	        $sql = "SELECT
	                    CASE WHEN itrid IN (1, 3)
	                    	THEN e.estdescricao
	                    	ELSE m.mundescricao
	                    END as descricao
	                FROM
	                    par.instrumentounidade i
	                LEFT JOIN territorios.municipio m ON m.muncod = i.muncod
	                LEFT JOIN territorios.estado    e ON e.estuf  = i.estuf
	                WHERE
	                    inuid = {$inuid}";
	        $titulo = $db->pegaUm( $sql );
	        $arParam = array ( "queid" => $queid, "titulo" => "PAR 2010 (".$titulo.")" );
	        $qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
	        $sql = "INSERT INTO par.parquestionario (inuid, qrpid) VALUES ({$inuid}, {$qrpid})";
	        $db->executar( $sql );
	        $db->commit();
    	}else{
    		echo "<script>alert('Ação impossivel. Questionario inexistente.')</script>";
    		return false;
    	}
    }
    return $qrpid;
}

function pegaQrpidProFunc( $queid, $inuid, $adpid ){

	global $db;

    $sql = "SELECT
            	qp.qrpid
            FROM
            	par.pfquestionario qp
            INNER JOIN questionario.questionarioresposta qr ON qr.qrpid = qp.qrpid
            WHERE
            	qp.adpid = {$adpid}
            	AND qr.queid = {$queid}";
    $qrpid = $db->pegaUm( $sql );

    if(!$qrpid){
    	$sql = "SELECT DISTINCT
    				'true'
    			FROM
            		questionario.questionario
            	WHERE
            		queid = ".$queid;
    	$testaQueid = $db->pegaUm($sql);
    	if( $testaQueid ){
	        $sql = "SELECT
	                    CASE WHEN itrid IN (1, 3)
	                    	THEN e.estdescricao
	                    	ELSE m.mundescricao
	                    END as descricao
	                FROM
	                    par.instrumentounidade i
	                LEFT JOIN territorios.municipio m ON m.muncod = i.muncod
	                LEFT JOIN territorios.estado    e ON e.estuf  = i.estuf
	                WHERE
	                    inuid = {$inuid}";
	        $titulo = $db->pegaUm( $sql );
	        if( $queid == QUESTIONARIOPROMUN ){
	        	$arParam = array ( "queid" => $queid, "titulo" => "PAR 2010 / Programa Mun (".$titulo.")" );
	        } else {
	        	$arParam = array ( "queid" => $queid, "titulo" => "PAR 2010 / Programa Est (".$titulo.")" );
	        }
	        $qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
	        $sql = "INSERT INTO par.pfquestionario (adpid, qrpid) VALUES ({$adpid}, {$qrpid})";
	        $db->executar( $sql );
	        $db->commit();
    	}else{
    		echo "<script>alert('Ação impossivel. Questionario inexistente.')</script>";
    		return false;
    	}
    }
    return $qrpid;
}

function pegaQrpidPAC( $preid, $queid ){

	include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";

    global $db;

    $sql = "SELECT
            	po.qrpid as qrpid,
            	po.predescricao as predescricao
            FROM
            	obras.preobra po
            LEFT JOIN questionario.questionarioresposta q ON q.qrpid = po.qrpid
            WHERE
            	po.preid = {$preid}
            	AND q.queid = {$queid}";

    $dados = $db->pegaLinha( $sql );

    if( empty( $dados['qrpid'] ) ){
        $arParam = array ( "queid" => $queid, "titulo" => "OBRAS (".$preid." - ".$dados['predescricao'].")" );
        $qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
        $sql = "UPDATE
                    obras.preobra
            	SET
                    qrpid = {$qrpid}
            	WHERE
                    preid = {$preid}";
    	$db->executar( $sql );
    	$db->commit();
    } else {
    	$qrpid = $dados['qrpid'];
    }
    return $qrpid;
}

function pegaQrpidAnalisePAC( $preid, $queid ){

	include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";

    global $db;

    $sql = "SELECT
            	po.qrpid as qrpid
            FROM
            	obras.preobraanalise po
            LEFT JOIN questionario.questionarioresposta q ON q.qrpid = po.qrpid
            WHERE
            	po.preid = {$preid}
            	AND q.queid = {$queid}";

    $dados = $db->pegaLinha( $sql );

    if( empty( $dados['qrpid'] ) ){
        $arParam = array ( "queid" => $queid, "titulo" => "OBRAS (".$preid.")" );
        $qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
        $sql = "UPDATE
                    obras.preobraanalise
            	SET
                    qrpid = {$qrpid}
            	WHERE
                    preid = {$preid}";
    	$db->executar( $sql );
    	$db->commit();
    } else {
    	$qrpid = $dados['qrpid'];
    }
    return $qrpid;
}


function pegaQrpidEja( $queid, $inuid, $adpid ){

	global $db;

	include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";

    $sql = "SELECT
            	qp.qrpid
            FROM
            	par.pfquestionario qp
            INNER JOIN questionario.questionarioresposta qr ON qr.qrpid = qp.qrpid
            WHERE
            	qp.adpid = {$adpid}
            	AND qr.queid = {$queid}";
    $qrpid = $db->pegaUm( $sql );

    if(!$qrpid){
    	$sql = "SELECT DISTINCT
    				'true'
    			FROM
            		questionario.questionario
            	WHERE
            		queid = ".$queid;
    	$testaQueid = $db->pegaUm($sql);
    	if( $testaQueid ){
	        $sql = "SELECT
	                    CASE WHEN itrid IN (1, 3)
	                    	THEN e.estdescricao
	                    	ELSE m.mundescricao
	                    END as descricao
	                FROM
	                    par.instrumentounidade i
	                LEFT JOIN territorios.municipio m ON m.muncod = i.muncod
	                LEFT JOIN territorios.estado    e ON e.estuf  = i.estuf
	                WHERE
	                    inuid = {$inuid}";
	        $titulo = $db->pegaUm( $sql );
	        if($inuid == 2){
	        	$arParam = array ( "queid" => $queid, "titulo" => "EJA / Programa Mun (".$titulo.")" );
	        } else {
	        	$arParam = array ( "queid" => $queid, "titulo" => "EJA / Programa Est (".$titulo.")" );
	        }
	        $qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
	        $sql = "INSERT INTO par.pfquestionario (adpid, qrpid) VALUES ({$adpid}, {$qrpid})";
	        $db->executar( $sql );
	        $db->commit();
    	}else{
    		echo "<script>alert('Ação impossivel. Questionario inexistente.')</script>";
    		return false;
    	}
    }
    return $qrpid;
}

function pegaQrpidMaisMedicos( $rqmid, $queid ){
	global $db;

    include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";

   	if($queid){
   		$aryWhere[] = "qr.queid = {$queid}";
   	}

   	if($rqmid){
   		$aryWhere[] = "rm.rqmid = {$rqmid}";
   	}

    $sql = "SELECT			rm.qrpid
            FROM		   	par.respquestaomaismedico rm
            INNER JOIN     	questionario.questionarioresposta qr ON qr.qrpid = rm.qrpid
            				".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."";

    $qrpid = $db->pegaUm($sql);

    if(!$qrpid){
    	$sql = "SELECT 	DISTINCT 'true'
    			FROM	questionario.questionario
            	WHERE	queid = {$queid}";

    	$testaQueid = $db->pegaUm($sql);

    	if($testaQueid){
	        $arParam = array ("queid" => $queid, "titulo" => "Mais Médicos (".$rqmid.")");
	        $qrpid = GerenciaQuestionario::insereQuestionario( $arParam );

	        $sql = "UPDATE	 	par.respquestaomaismedico
	            	SET	        qrpid = {$qrpid}
	            	WHERE       rqmid = {$rqmid}";

	        $db->executar($sql);
	        $db->commit();
    	} else {
    		echo "<script>alert('Ação impossivel. Questionario inexistente.')</script>";
    		return false;
    	}
    }
    return $qrpid;
}


function cabecalho()
{
    global $db;

	if($_SESSION['par']['itrid'] == 1) {
		$sql = "
		    SELECT
                estdescricao as descricao
            FROM
                territorios.estado
            WHERE
                estuf = '".$_SESSION['par']['estuf']."';
	    ";

		$descricao = $db->pegaUm( $sql );

		$desc = "
		    <tr>
				<td width=\"30%\" style=\"text-align: right;\" class=\"SubTituloDireita\">Descrição:</td>
				<td width=\"70%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".$descricao."</td>
			</tr>
        ";
	} else {
		$sql = "
            SELECT
				estuf,
                mundescricao as descricao
            FROM
                territorios.municipio
            WHERE
                muncod = '".$_SESSION['par']['muncod']."';
	    ";

		$municipio = $db->pegaLinha( $sql );

		$desc = "
		    <tr>
				<td width=\"30%\" style=\"text-align: right;\" class=\"SubTituloDireita\">Município:</td>
				<td width=\"70%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".$municipio['descricao']."</td>
			</tr>
	    ";
	}

	if(!$_SESSION['par']['preid']) {
		$ptodescricao = '-';
	} else {
		$sql = "
		    SELECT
				ptodescricao
			FROM
				obras.pretipoobra pto
                INNER JOIN obras.preobra po ON po.ptoid = pto.ptoid
			WHERE
				po.preid = {$_SESSION['par']['preid']}";

		$ptodescricao = $db->pegaUm($sql);

		$sqlEmp = "SELECT saldo FROM par.vm_saldo_empenho_por_obra WHERE preid = {$_SESSION['par']['preid']}";
		$valor = $db->pegaUm($sqlEmp);

		if($valor > 0) {
			$emp = "
			    <tr>
					<td width=\"30%\" style=\"text-align: right;\" class=\"SubTituloDireita\">Valor Empenhado:</td>
					<td width=\"70%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">
						R$ ".number_format($valor,2,',','.')."
					</td>
				</tr>
            ";
		}
	}

	if($_SESSION['par']['prog'] == 'proinf') {
		$tipoobra = "
			<tr>
    			<td width=\"30%\" style=\"text-align: right;\" class=\"SubTituloDireita\">Tipo Obra:</td>
				<td width=\"70%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".$ptodescricao."</td>
			</tr>
	    ";
	}

	$preid = $_REQUEST['preid'] ? $_REQUEST['preid'] : $_SESSION['par']['preid'];

	if($preid) {
		$docid = prePegarDocid($preid);
		$esdid = prePegarEstadoAtual($docid);

		$reformulacaoMI = verificaMi($preid);

		if($reformulacaoMI) {
			$mi = '
			    <div style="color: red; position: absolute; top: 70px; right: 60px; float: right; z-index:1;">
					<img border="0" title="Obra de Metodologia Inovadora." src="../imagens/carimbo-mi.png">
				</div>
		    ';
		}

		$sql = "
	       SELECT
		        aoscodsituacao
		    FROM
			    par.adesaoobraspacsituacao
		    WHERE
			    preid = " . $preid . "
			    AND aoscodsituacao = 110;
	    ";
// dbg($sql);
		$situacaoObra = $db->pegaUm($sql);
		//@todo retirar ao subir
		$EmEeformulacaoMIConv = verificaMiConvencional($preid);

		$comDistrato = testaObraMIDestrato( $preid );
// 		ver($_SERVER);
		if(!$EmEeformulacaoMIConv && !$comDistrato && !in_array($_SERVER['SERVER'], Array('simec-local', 'simec-d', 'simec-d.mec.gov.br') ) )
		{
			if($situacaoObra) {
				$cotratoassinado = "
				    <tr>
						<td width=\"30%\" style=\"text-align: right;\" class=\"SubTituloDireita\">Tramitação:</td>
						<td width=\"70%\" style=\"color: red; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">
	    					<b>A obra não pode ser encaminhada para reformulação pois se encontra com o contrato assinado.</b>
						</td>
					</tr>";
			}
		}
		$usucpf = $_SESSION['usucpf'];
		$perfilGeral = pegaPerfilGeral($usucpf);

		if($db->testa_superuser() || in_array(PAR_PERFIL_ADM_OBRAS, $perfilGeral) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfilGeral) || in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfilGeral) || in_array(471, $perfilGeral)) {
		    $visualizaDiligencias = true;
		} else {
		    $visualizaDiligencias = false;
		}

		if($docid && $visualizaDiligencias) {

		    $sqlBuscaQntDiligencia = "
			    SELECT
                	 COUNT(*)
                FROM
                	workflow.historicodocumento hsd
                	INNER JOIN workflow.documento doc ON hsd.docid = doc.docid
                	INNER JOIN workflow.acaoestadodoc aed ON hsd.aedid = aed.aedid
                WHERE
                	doc.docid = " . $docid . "
                	AND (aed.esdiddestino = " . WF_PAR_EM_DILIGENCIA . " OR aed.esdiddestino = " . WF_TIPO_EM_CORRECAO . ")
		    ";

		    $qntDiligencia = $db->pegaUm($sqlBuscaQntDiligencia);

		    if($db->pegaUm($sqlBuscaQntDiligencia) > 0 || !empty($qntDiligencia)) {
		      $btnVisuliza = "<a href=\"#\" id=\"btnVisualizaComentariosDiligencias\" class=\"mais\" onclick=\"visualizaComentariosDiligencia(" . $docid . ")\"><img src=\"../imagens/mais.gif\" alt=\"Mais\" title=\"Mais\" /></a><div id=\"comentariosDiligencia\"></div>";
		    } else {
		       $btnVisuliza = "0";
		    }

		    $qntDiligencia = "
			    <tr>
                    <td width=\"30%\" style=\"text-align: right;\" class=\"SubTituloDireita\">Quantidade de diligências:</td>
                    <td width=\"70%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">" . $qntDiligencia . "&nbsp;" . $btnVisuliza ."</td>
                </tr>
            ";
		}

	}

	echo "
	    <table align=\"center\" class=\"Tabela\" cellpadding=\"2\" cellspacing=\"1\">
			<tbody>
				<tr>
					<td width=\"30%\" style=\"text-align: right;\" class=\"SubTituloDireita\">UF:</td>
					<td width=\"70%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">" . (($municipio['estuf']) ? $municipio['estuf'] : $_SESSION['par']['estuf']) . "</td>
				</tr>
				{$desc}
				{$tipoobra}
				{$emp}
				{$cotratoassinado}
				{$mi}
				{$qntDiligencia}
			</tbody>
		</table>
	";
}

function possuiPerfil( $pflcods ){

	global $db;

	if($db->testa_superuser()){
		return true;
	}

	if ( is_array( $pflcods ) ){
		$pflcods = array_map( "intval", $pflcods );
		$pflcods = array_unique( $pflcods );
	} else {
		$pflcods = array( (integer) $pflcods );
	} if ( count( $pflcods ) == 0 ) {
		return false;
	}
	$sql = "SELECT
					count(*)
			FROM seguranca.perfilusuario
			WHERE
				usucpf = '" . $_SESSION['usucpf'] . "' and
				pflcod in ( " . implode( ",", $pflcods ) . " ) ";
	return $db->pegaUm( $sql ) > 0;
}

//Essa função pegaPerfil não funciona direito
function pegaPerfil($usucpf){

	global $db;
	$sql = "SELECT
				pu.pflcod
			FROM seguranca.perfil AS p
			LEFT JOIN seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
			WHERE
				p.sisid = '{$_SESSION['sisid']}'
				AND pu.usucpf = '$usucpf'";


	$pflcod = $db->pegaUm( $sql );
	return $pflcod;
}

function pegaArrayPerfil($usucpf){

	global $db;

	$sql = "SELECT
				pu.pflcod
			FROM
				seguranca.perfil AS p
			LEFT JOIN seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
			WHERE
				p.sisid = '{$_SESSION['sisid']}'
				AND pu.usucpf = '$usucpf'";

	$pflcod = $db->carregarColuna( $sql );

	return $pflcod;
}

function pegaMunicipioAssociado($perfil){

	global $db;
	
	if( is_array($perfil) ){
		$perfil = implode(', ', $perfil);
	}

	$sql = "SELECT
				ur.muncod,
				m.estuf
			FROM
				par.usuarioresponsabilidade ur
			INNER JOIN territorios.municipio m ON ur.muncod = m.muncod
			WHERE
				usucpf = '{$_SESSION['usucpf']}'
				AND rpustatus = 'A'
				AND pflcod in ($perfil) ";
	$municipio = $db->carregar($sql);
	
	if($municipio[0]){
		return $municipio;
	}

	return false;
}

function pegaMunicipioAssociadoAnalista($perfil){
	global $db;

	$sql = "SELECT
				ur.muncod,
				ur.estuf
			FROM
				par.usuarioresponsabilidade ur
			LEFT JOIN territorios.municipio m ON ur.muncod = m.muncod
			WHERE
				usucpf = '{$_SESSION['usucpf']}'
				AND rpustatus = 'A'
				AND pflcod IN ('".implode(" ', '", $perfil)."') ";
//ver($sql, d);
	$municipio = $db->carregar($sql);

	if($municipio[0]){
		return $municipio;
	}

	return false;
	/*
	global $db;

	$sql = "SELECT
				muncod,
				estuf
			FROM
				par.analistasassociados
			WHERE
				usucpf = '{$_SESSION['usucpf']}'";
	$municipio = $db->carregar($sql);

	if($municipio[0]){
		return $municipio;
	}

	return false;
	*/
}

function pegaEstadoAssociado($perfil){

	global $db;

	$sql = "SELECT
				estuf
			FROM
				par.usuarioresponsabilidade
			WHERE
				usucpf = '{$_SESSION['usucpf']}'
				AND rpustatus = 'A'
				AND pflcod = '{$perfil}' ";
	$estado = $db->carregar($sql);
	if($estado[0]){
		return $estado;
	}

	return false;
}

function delimitador($texto, $valor = null){

// 	if(!ereg('[^0-9]',$texto)){
	if(!preg_match('[^0-9]',$texto)){
		return $texto;
	}

	if(!$valor){
		$valor = 280;
	}

	if(strlen($texto) > $valor){
			$texto = substr($texto,0,$valor).'...';
	}

	return $texto;
}


/*** FUNÇÕES DE BLOQUEIO DA ATUALIZAÇÃO DO PAR ***/

function bloqueioEnvioAnaliseSubacaoAtualizacao(){

	global $db;

	if( $db->testa_superuser() ){
		return true;
	}

	// Se Municipal 20130901
	if( $_SESSION['par']['itrid'] == 2 && date("Ymd") >= 20130901 ){
		return "Não é possível enviar para análise. O prazo para atualização do PAR esgotou.";
	}else{
	// se Estatual
		return true;
	}
	return true;
}

function verificaEstadoSubacao( $sbaid, $esdids ){

	global $db;

	$sql = "SELECT
				true
			FROM
				par.subacao sba
			INNER JOIN workflow.documento doc ON doc.docid = sba.docid
			WHERE
				sbaid = $sbaid
				AND esdid IN ( ".implode(', ',$esdids)." )";

	return $db->pegaUm($sql);
}

function bloqueioEnvioAnaliseObrasAtualizacao( $ano, $preid ){

	global $db;

	$data = date("Ymd");
	//20130901
	if( $_SESSION['par']['itrid'] ){
		$itrid = $_SESSION['par']['itrid'];
	} else {
		$sql = "SELECT CASE WHEN preesfera = 'E' THEN 1 ELSE 2 END as esfera FROM obras.preobra p WHERE preid = ".$preid;
		$itrid = $db->pegaUm($sql);
	}

	if( $itrid == 2 ){
		if( $ano <= '2013' && $data >= 20130901 ){
			return false;
		}
	}
	return true;
}

/*******************************************/

/*** FUNÇÕES WORKFLOW PAR 2010***/


function eaVerificaEstado( $esdid ){

	global $db;

	$sql = "SELECT
				esdid
			FROM
				workflow.estadodocumento
			WHERE
				esdid = {$esdid}";

	return $db->pegaUm( $sql );

}

function eaCriarDocumento( $monid ) {

	global $db;

	require_once APPRAIZ . 'includes/workflow.php';

	$docid = eaPegarDocid( $monid );

	if( !$docid ) {

		// recupera o tipo do documento
		$tpdid = EA_TIPO_DOCUMENTO;

		// descrição do documento
		$docdsc = "Fluxo Escola Ativa - n°" . $monid;

		// cria documento do WORKFLOW
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );

		// atualiza pap do EMI
		$sql = "UPDATE
					escolaativa.monitoramento
				SET
					docid = {$docid}
				WHERE
					monid = {$monid}";

		$db->executar( $sql );
		//$db->commit();
	}

	return $docid;

}

function eaPegarDocid( $monid ) {

	global $db;

	$sql = "SELECT
				docid
			FROM
				escolaativa.monitoramento
			WHERE
			 	monid = " . (integer) $monid;

	return (integer) $db->pegaUm( $sql );

}

function eaPegarEstadoAtual( $monid ) {

	global $db;

	$docid = eaPegarDocid( $monid );

	$sql = "SELECT
				ed.esdid
			FROM
				workflow.documento d
			INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
			WHERE
				d.docid = " . $docid;

	$estado = (integer) $db->pegaUm( $sql );

	return $estado;

}

/************FIM***************/

/*** FUNÇÕES WORKFLOW PRE OBRA***/

function preVerificaEstado( $esdid ){

	global $db;

	$sql = "SELECT
				esdid
			FROM
				workflow.estadodocumento
			WHERE
				esdid = {$esdid}";

	return $db->pegaUm( $sql );

}

function preCriarDocumento( $preid, $tpdid = WF_FLUXO_PRO_INFANCIA ) {

	global $db;

	require_once APPRAIZ . 'includes/workflow.php';

	$docid = prePegarDocid( $preid );

	if( !$docid ) {

		// descrição do documento
		switch ($tpdid){
			case WF_FLUXO_PRO_INFANCIA:
				$docdsc = "Fluxo pró infância";
				break;
			case WF_FLUXO_PRONATEC:
				$docdsc = "Fluxo Pronatec";
				break;
			case WF_FLUXO_OBRAS_PAR:
				$docdsc = "Fluxo Par";
				break;
		}

		// cria documento do WORKFLOW
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );

		// atualiza pap do EMI
		$sql = "UPDATE
					obras.preobra
				SET
					docid = {$docid}
				WHERE
					preid = {$preid}";

		$db->executar( $sql );
		$db->commit();
	}

	return $docid;

}

function prePegarDocid( $preid ) {

	global $db;

	$sql = "SELECT
				docid
			FROM
				obras.preobra
			WHERE
			 	preid = " . (integer) $preid;

	return (integer) $db->pegaUm( $sql );

}

function prePegarEstadoAtual( $docid ) {

	global $db;

	$sql = "SELECT
				ed.esdid
			FROM
				workflow.documento d
			INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
			WHERE
				d.docid = " . $docid;

	$estado = (integer) $db->pegaUm( $sql );

	return $estado;

}

function verificaGrupoMunicipio( $preid ){

	global $db;

	$sql = "SELECT
				tpm.tpmid
			FROM
				obras.preobra po
			INNER JOIN territorios.muntipomunicipio mtp ON mtp.muncod = po.muncod
			INNER JOIN territorios.tipomunicipio    tpm ON tpm.tpmid  = mtp.tpmid
														   AND tpm.tpmstatus = 'A'
														   AND tpm.gtmid = 7
			WHERE
				po.preid = ".$preid;
	$grupo = $db->pegaUm($sql);

	return $grupo;
}

function pegaTiposMunicipio( $muncod ){

	global $db;

	$sql = "SELECT
				tpm.tpmid
			FROM
				territorios.muntipomunicipio mtp
			INNER JOIN territorios.tipomunicipio tpm ON tpm.tpmid = mtp.tpmid
				AND tpm.tpmstatus = 'A'
			WHERE
				mtp.muncod = '{$muncod}'";
	$tipos = $db->carregarColuna($sql);

	return $tipos;
}

function verificaGrupoMunicipioMUNCOD( $muncod, $grupo1 = null ){

	global $db;

	$sql = "SELECT
				tpm.tpmid
			FROM
				territorios.muntipomunicipio mtp
			INNER JOIN territorios.tipomunicipio tpm ON tpm.tpmid = mtp.tpmid
													 	AND tpm.tpmstatus = 'A'
													 	AND tpm.gtmid = 7
			WHERE
				mtp.muncod = '{$muncod}'";
	$grupo = $db->pegaUm($sql);
//	echo "<script>alert('".$grupo."')</script>";

	if( $grupo1 ){
		if( $grupo == TIPOMUN_GRUPO1 || $grupo == TIPOMUN_GRUPO1_2011 ){
			return true;
		}else{
			return false;
		}
	} else {
		if( $grupo == TIPOMUN_GRUPO1 || $grupo == TIPOMUN_GRUPO2 ){
			return true;
		}else{
			return false;
		}
	}
}

function verificaPendenciasObras( $preid )
{
	global $db;

	$qtdObra = verificaObraEmenda($preid);

	if( $qtdObra > 0 ){
		return array(
			'erro' 		=> false,
			'mensagem' 	=> ''
		);
	}

	$sql = "SELECT
				inu.inuid,
				pre.preesfera,
				pre.estuf
			FROM
				obras.preobra pre
			INNER JOIN par.instrumentounidade inu ON inu.muncod = pre.muncod  and mun_estuf is not null
			WHERE
				preid = {$preid}
				and pre.preesfera = 'M'

			UNION ALL

			SELECT
				inu.inuid,
				pre.preesfera,
				inu.estuf
			FROM
				obras.preobra pre
			INNER JOIN par.instrumentounidade inu ON inu.estuf = pre.estuf --and inu.muncod is null
			WHERE
				preid = {$preid}
				and pre.preesfera = 'E'";

	$result = $db->carregar($sql);
	$inuid = $result[0]['inuid'];
	$esfera = $result[0]['preesfera'];
	$estuf = $result[0]['estuf'];

	if( $esfera == 'E' || $estuf == 'DF' ){
		$sql = "SELECT estado,stoid, COUNT(1) AS qtdbloqueio FROM (
					SELECT
						o.obrid,
						o.estuf AS estado,
						o.situacaoobra AS stoid,
						CASE WHEN o.situacaoobra IN (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") AND desterminodeferido IS NOT NULL AND desterminodeferido >= now() THEN 0
							WHEN o.situacaoobra IN (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") AND coalesce(o.diasprimeiropagamento, o.diasinclusao) > 540 THEN 1
							ELSE 0 end as bloqueiolicitacao,
						CASE WHEN o.situacaoobra IN (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.") and o.diasultimaalteracao > 60 THEN 1 ELSE 0 END as bloqueioexecucaoparalisada

					FROM
						obras2.vm_obras_situacao_estadual o
					WHERE
						o.inuid = {$inuid}
						AND o.situacaoobra IN (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.", ".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.")
				) t WHERE bloqueiolicitacao = 1 or bloqueioexecucaoparalisada = 1
				GROUP BY estado, stoid";

		$dadosObra = $db->pegaLinha($sql);
	} else {
		$sql = "SELECT estado,stoid, COUNT(1) AS qtdbloqueio FROM (
					SELECT
						o.obrid,
						o.estuf AS estado,
						o.situacaoobra AS stoid,
						CASE WHEN o.situacaoobra IN (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") AND desterminodeferido IS NOT NULL and desterminodeferido >= now() THEN 0
							WHEN o.situacaoobra IN (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") AND coalesce(o.diasprimeiropagamento, o.diasinclusao) > 540 THEN 1
							ELSE 0 END AS bloqueiolicitacao,
						CASE WHEN o.situacaoobra IN (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.") AND o.diasultimaalteracao > 60 THEN 1 ELSE 0 END AS bloqueioexecucaoparalisada

					FROM
						obras2.vm_obras_situacao_municipal o
					WHERE
						o.inuid = {$inuid}
						AND o.situacaoobra IN (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.", ".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.")
				) t WHERE bloqueiolicitacao = 1 OR bloqueioexecucaoparalisada = 1
				GROUP BY estado, stoid";
		$dadosObra = $db->carregar($sql);
	}

	if(is_array($dadosObra) && count($dadosObra))
	{
		return array(
			'erro' 		=> true,
			'mensagem' 	=> 'Existem obras com pendências no monitoramento de obras'
		);

	}
	else
	{
		return array(
			'erro' 		=> false,
			'mensagem' 	=> ''
		);
	}
}

function retornaTipoOrigemObra($preid)
{
	global $db;

	$origemobra = $db->pegaLinha("
					SELECT
						pre.tooid,
						too.toodescricao
					FROM
						obras.preobra pre
					INNER JOIN obras.tipoorigemobra  too ON too.tooid = pre.tooid
					WHERE preid = {$preid}");
	if($origemobra['toodescricao'])
	{
		return $origemobra['toodescricao'];
	}
	else
	{
		return '';
	}
}

/*
 * Condição para cadastramento/reformulação/diligencia para análise
 * */
function verificaWFpreObraPAR($preid)
{

	global $db;
	$docid = prePegarDocid($preid);
	$esdid = prePegarEstadoAtual($docid);
	$descOrigemObra = retornaTipoOrigemObra($preid);
	$arrTipoObras = $db->pegaLinha("select pre.estuf ,pre.tooid, pto.ptoclassificacaoobra, sobano, pre.preesfera
									from obras.preobra pre
										inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
										inner join par.subacaoobra sbo on  pre.preid = sbo.preid AND pre.prestatus = 'A' AND pre.preidpai IS NULL AND pre.tooid <> 1
									where
										pre.preid = {$preid}
									    and pre.prestatus = 'A'
									    and pto.ptostatus = 'A'");

	$tooid = $arrTipoObras['tooid'];
	$anoObra = $arrTipoObras['sobano'];
	$esfera = $arrTipoObras['preesfera'];
	$estado = $arrTipoObras['estuf'];

	// Caso seja uma reformulação libera a tramitação
	if($esdid == WF_PAR_OBRA_EM_REFORMULACAO )
	{
		return true;
	}

	// Verifica pendencias das obras
	$retornoVerificacaoPAR = verificaWFpreObra( $preid );

	if( ($retornoVerificacaoPAR !== true) && ($retornoVerificacaoPAR !== false) )
	{
		return $retornoVerificacaoPAR;

	} elseif( $retornoVerificacaoPAR == false )
	{
		return false;
	}


	if( $esfera == 'E' )
	{
		// Verifica pendencias do obras 2.0
		if( in_array( $esdid ,  array(WF_PAR_EM_DILIGENCIA, WF_PAR_EM_CADASTRAMENTO ) ) )
		{
			$result = verificaPendenciasObras( $preid );

			if($result['erro'])
			{
				return $result['mensagem'];
			}
		}
	}
	// Se municipal, pode se for fase de diligencia ou reformulacao ( APLICAR ESTAS REGRAS PARA O DF)
	elseif( $esfera == 'M' )
	{
		if( in_array( $esdid ,  array(WF_PAR_EM_DILIGENCIA, WF_PAR_EM_CADASTRAMENTO ) ) )
		{
			if( $esdid == WF_PAR_EM_CADASTRAMENTO )
			{
				if( in_array( $anoObra, array( 2011,2012,2013 ) ) )
				{
					return 'Prazo de envio encerrado';
				}
			}
			// Verifica pendencias do obras 2.0
			$result = verificaPendenciasObras( $preid );

			if($result['erro'])
			{
				return $result['mensagem'];
			}
		}

	}

	return true;
}

function verificaWFpreObraPAC($preid)
{
	global $db;

	if( $_SESSION['usucpf'] == '' ){
		return true;
	}

	$docid = prePegarDocid($preid);
	$esdid = prePegarEstadoAtual($docid);

    $esfera = $db->pegaUm("select preesfera from obras.preobra where preid = '$preid'");
    $obraEmenda = verificaObraEmenda($preid);

    if(date('Ymd') >= '20140515' && $esfera == 'M' && !$obraEmenda && ( $esdid != WF_TIPO_EM_REFORMULACAO && $esdid != WF_TIPO_EM_CORRECAO ) ){
        return "Prazo expirado.";
    }

    $possuiEmpenho = false;
    if($preid){

    	// obras pac que são proinfância pode tramitar de diligência para analise e cadastramento para analise
    	$sql = "SELECT COUNT(p.preid)
			FROM obras.preobra  p
			INNER JOIN obras.pretipoobra pto ON pto.ptoid = p.ptoid
			WHERE 	p.tooid = 1
					AND pto.ptoclassificacaoobra = 'P'
					AND p.preid = {$preid}
			";
    	$liberaTramitacaoPAC = $db->pegaUm($sql);

        $sql = "select count(*)
                from par.empenhoobra eo
                    inner join par.empenho e on e.empid = eo.empid and eobstatus = 'A' and trim(e.empsituacao) != 'CANCELADO' and empstatus = 'A'
                where preid = '{$preid}'";
        $possuiEmpenho = $db->pegaUm($sql);
    }

    // Verifica obras pendentes
    if(isset($_SESSION['par']['itrid']) && !$liberaTramitacaoPAC){
        $possuiPendenciaObras = $_SESSION['par']['itrid'] == 2 ? getObrasPendentesPAR($_SESSION['par']['muncod']) : getObrasPendentesPAR(null, $_SESSION['par']['estuf']);

        if(!empty($possuiPendenciaObras)  && !$possuiEmpenho && !$obraEmenda && ( $esdid != WF_TIPO_EM_REFORMULACAO && $esdid != WF_TIPO_EM_CORRECAO )
        ){
            return "Existem obras com pendências no monitoramento de obras.";
        }
    }

	// Verifica pendencias das obras
	$retornoVerificacaoPAC = verificapendenciaObrasPAC($preid);
	//$retornoVerificacaoPAC = true;

	if( ($retornoVerificacaoPAC !== true) && ($retornoVerificacaoPAC !== false) )
	{
		return $retornoVerificacaoPAC;

	} elseif( $retornoVerificacaoPAC == false )
	{
		return false;
	}


	$descOrigemObra = retornaTipoOrigemObra($preid);
	$arrTipoObras = $db->pegaLinha("select pre.estuf ,pre.tooid, pto.ptoclassificacaoobra, preanoselecao, pre.preesfera
									from obras.preobra pre
										inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
									where
										pre.preid = {$preid}
									    and pre.prestatus = 'A'
									    and pto.ptostatus = 'A'");
	$tooid = $arrTipoObras['tooid'];
	$preanoselecao = $arrTipoObras['preanoselecao'];
	$esfera = $arrTipoObras['preesfera'];
	$estado = $arrTipoObras['estuf'];

	// Se estadual pode tramitar, (NÃO APLICAR CASO DF)
	if( ($esfera == 'E') && ($estado != 'DF') )
	{
		return true;
	}
	// Se municipal, pode se for fase de diligencia ou reformulacao ( APLICAR ESTAS REGRAS PARA O DF)
	elseif(($esfera == 'M') || ($estado == 'DF') )
	{

		if( ($esdid == WF_TIPO_EM_REFORMULACAO) ||( $esdid == WF_TIPO_EM_CORRECAO ) )
		{
			if(($esdid == WF_TIPO_EM_CORRECAO))
			{
				$retornoVerificacao = verificaPeriodoEnviarDiligencia( $preid );

				if( ($retornoVerificacao !== true) && ($retornoVerificacao !== false) )
				{
					return $retornoVerificacao;
				}
				elseif( $retornoVerificacao == false )
				{
					return false;
				}
			}
			return true;
		}
		elseif($esdid == WF_TIPO_EM_CADASTRAMENTO )
		{
			//return 'Prazo de envio encerrado';
			return true;
		}

	}
    return true;
}

function WF_COND_subacao_elaboracao_analise($sbaid)
{
	global $db;

    $esfera = $db->pegaUm("select preesfera from obras.preobra where preid = '$preid'");
    $obraEmenda = verificaObraEmenda($preid);

    if(date('Ymd') >= '20140515' && $esfera == 'M' && !$obraEmenda){
        return "Prazo expirado.";
    }

    $possuiEmpenho = false;
    if($preid){

    	// obras pac que são proinfância pode tramitar de diligência para analise e cadastramento para analise
    	$sql = "SELECT COUNT(p.preid)
			FROM obras.preobra  p
			INNER JOIN obras.pretipoobra pto ON pto.ptoid = p.ptoid
			WHERE 	p.tooid = 1
					AND pto.ptoclassificacaoobra = 'P'
					AND p.preid = {$preid}
			";
    	$liberaTramitacaoPAC = $db->pegaUm($sql);

        $sql = "select count(*)
                from par.empenhoobra eo
                    inner join par.empenho e on e.empid = eo.empid and trim(e.empsituacao) != 'CANCELADO' and eobstatus = 'A' and empstatus = 'A'
                where preid = '{$preid}'";
        $possuiEmpenho = $db->pegaUm($sql);
    }

    // Verifica obras pendentes
    if(isset($_SESSION['par']['itrid']) && !$liberaTramitacaoPAC){
        $possuiPendenciaObras = $_SESSION['par']['itrid'] == 2 ? getObrasPendentesPAR($_SESSION['par']['muncod']) : getObrasPendentesPAR(null, $_SESSION['par']['estuf']);

        if(!empty($possuiPendenciaObras)  && !$possuiEmpenho && !$obraEmenda
        ){
            return "Existem obras com pendências no monitoramento de obras.";
        }
    }

	// Verifica pendencias das obras
	$retornoVerificacaoPAC = verificapendenciaObrasPAC($preid);
	//$retornoVerificacaoPAC = true;

	if( ($retornoVerificacaoPAC !== true) && ($retornoVerificacaoPAC !== false) )
	{
		return $retornoVerificacaoPAC;

	} elseif( $retornoVerificacaoPAC == false )
	{
		return false;
	}

	$docid = prePegarDocid($preid);
	$esdid = prePegarEstadoAtual($docid);
	$descOrigemObra = retornaTipoOrigemObra($preid);
	$arrTipoObras = $db->pegaLinha("select pre.estuf ,pre.tooid, pto.ptoclassificacaoobra, preanoselecao, pre.preesfera
									from obras.preobra pre
										inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
									where
										pre.preid = {$preid}
									    and pre.prestatus = 'A'
									    and pto.ptostatus = 'A'");
	$tooid = $arrTipoObras['tooid'];
	$preanoselecao = $arrTipoObras['preanoselecao'];
	$esfera = $arrTipoObras['preesfera'];
	$estado = $arrTipoObras['estuf'];

	// Se estadual pode tramitar, (NÃO APLICAR CASO DF)
	if( ($esfera == 'E') && ($estado != 'DF') )
	{
		return true;
	}
	// Se municipal, pode se for fase de diligencia ou reformulacao ( APLICAR ESTAS REGRAS PARA O DF)
	elseif(($esfera == 'M') || ($estado == 'DF') )
	{

		if( ($esdid == WF_TIPO_EM_REFORMULACAO) ||( $esdid == WF_TIPO_EM_CORRECAO ) )
		{
			if(($esdid == WF_TIPO_EM_CORRECAO))
			{
				$retornoVerificacao = verificaPeriodoEnviarDiligencia( $preid );

				if( ($retornoVerificacao !== true) && ($retornoVerificacao !== false) )
				{
					return $retornoVerificacao;
				}
				elseif( $retornoVerificacao == false )
				{
					return false;
				}
			}
			return true;
		}
		elseif($esdid == WF_TIPO_EM_CADASTRAMENTO )
		{
			//return 'Prazo de envio encerrado';
			return true;
		}

	}
    return true;
}

function verificapendenciaObrasPAC($preid){

	global $db;
	$oPreObra = new PreObra();
	$oSubacaoControle = new SubacaoControle();

	$qrpid = pegaQrpidPAC( $preid, 43 );

	//regras para a tramitação do wf
	$boPlanilhaOrcamentaria = $oSubacaoControle->verificaPlanilhaOrcamentaria($preid, SIS_OBRAS, $preid);
	$pacDados 				= $oSubacaoControle->verificaTipoObra($preid, SIS_OBRAS);
	$pacFNDE 				= $oSubacaoControle->verificaObraFNDE($preid, SIS_OBRAS);
	$pacFotos 				= $oSubacaoControle->verificaFotosObra($preid, SIS_OBRAS);
	$pacDocumentos 			= $oSubacaoControle->verificaDocumentosObra($preid, SIS_OBRAS, $pacDados);

	if($pacFNDE == 'f'){
		$pacDocumentosTipoA = $oSubacaoControle->verificaDocumentosObra($preid, SIS_OBRAS, $pacDados, true);
	}
	$pacQuestionario 		= $oPreObra->verificaQuestionario($qrpid);
	$pacLatitude	 		= $oPreObra->verificaLatitudePreObra($preid);
	$pacCronograma 			= $oPreObra->verificaCronograma($preid);
	$boPlanilhaOrcamentaria['faltam'] = $boPlanilhaOrcamentaria['itcid'] - $boPlanilhaOrcamentaria['ppoid'];

	$perfil = pegaArrayPerfil($_SESSION['usucpf']);

	$boDocumento = false;
	$boDocumentoTipoA = false;
	if($pacDocumentosTipoA['arqid'] == $pacDocumentosTipoA['podid']){
		$boDocumentoTipoA = true;
	}
	if($pacDocumentos['arqid'] == $pacDocumentos['podid']){
		$boDocumento = true;
	}

	$grupo = verificaGrupoMunicipio( $preid );

	$arTipoCodigo = array();

	if($esdid == WF_TIPO_EM_CADASTRAMENTO || $esdid == WF_TIPO_EM_CORRECAO){
		if( $grupo == TIPOMUN_GRUPO1_2011 ){
			$arTipoObra = Array(
								Array('codigo' => 1, 'descricao' => 'Escola Infantil - Tipo A' ),
								Array('codigo' => 38, 'descricao' => 'Escola Infantil - Tipo B (Projeto Novo)'),
								Array('codigo' => 39, 'descricao' => 'Escola Infantil - Tipo C (Projeto Novo)'),
								Array('codigo' => 43, 'descricao' => 'Escola Proinfância B - Metodologias Inovadoras'),
								Array('codigo' => 42, 'descricao' => 'Escola Proinfância C - Metodologias Inovadoras'),
								Array('codigo' => 44, 'descricao' => 'Escola Proinfância B - Metodologias Inovadoras - Emendas'),
								Array('codigo' => 45, 'descricao' => 'Escola Proinfância C - Metodologias Inovadoras - Emendas')
							);
			$arTipoCodigo = array(1, 38, 39, 43, 42, 44, 45);
		} else {
			$arTipoObra = Array(
								Array('codigo' => 38, 'descricao' => 'Escola Infantil - Tipo B (Projeto Novo)'),
								Array('codigo' => 39, 'descricao' => 'Escola Infantil - Tipo C (Projeto Novo)'),
								Array('codigo' => 43, 'descricao' => 'Escola Proinfância B - Metodologias Inovadoras'),
								Array('codigo' => 42, 'descricao' => 'Escola Proinfância C - Metodologias Inovadoras'),
								Array('codigo' => 43, 'descricao' => 'Escola Proinfância B - Metodologias Inovadoras - Emendas'),
								Array('codigo' => 42, 'descricao' => 'Escola Proinfância C - Metodologias Inovadoras - Emendas')
							);
			$arTipoCodigo = array(38, 39, 43, 42, 44, 45);
		}

		// Verifico se a Obra está nos tipos A, B, C ou nos projetos novos.
		if(!in_array($pacDados, $arTipoCodigo) && $arrTipoObras['ptoclassificacaoobra'] == 'P'){
			return 'A Obra não está nos tipos A, B, C ou nos projetos novos!';
		}
	}


	/*
	 * O bloco a seguir foi criado para tratar obras que não foram enviadas para analise juntamente com o par.
	 * Caso o perfil seja municipal, o par esteja em analise e a obra esteja preenchida então ele pode tramitar.
	 *
	 * Victor Benzi - 27/07/2012
	 */
	$validacao = true;

	$testaGrupo = verificaGrupoMunicipio( $preid );

	if($validacao && $pacDados && $pacFotos >= 3 && $boDocumentoTipoA && $boDocumento && $pacQuestionario == 22 && $pacLatitude){
		if($pacFNDE == 't'){
			if( /*(*/str_replace(',','',number_format($boPlanilhaOrcamentaria['valor']) ) < $boPlanilhaOrcamentaria['minimo'] ||
// 			   str_replace(',','',number_format($boPlanilhaOrcamentaria['valor']) ) > $boPlanilhaOrcamentaria['maximo']) ||
			   	$boPlanilhaOrcamentaria['faltam'] > 0 ||
			   	$boPlanilhaOrcamentaria['ppoid'] != $boPlanilhaOrcamentaria['itcid'] || $pacCronograma == 0 )
			{
 				if( $boPlanilhaOrcamentaria['minimo'] != '' && $boPlanilhaOrcamentaria['maximo'] != ''){
					return false;
 		   		} else {
 		   			return true;
 		   		}
			}else{
				if( $testaGrupo != TIPOMUN_GRUPO3 ){
					return true;
				}else{
					return true;
				}
			}
		}else{
			if( $testaGrupo != TIPOMUN_GRUPO3 ){
				return true;
			}else{
				return true;
			}
		}
	}else{

		return false;
	}
}

function verifica_pendencias_preObra( $preid )
{
	global $db;

	$oPreObra = new PreObra();
	$oSubacaoControle = new SubacaoControle();

	$qrpid = pegaQrpidPAC( $preid, 43 );
	$sql = "SELECT ptoid FROM obras.preobra WHERE preid = $preid";
	$ptoid = $db->pegaUm( $sql );

	if( $oPreObra->verificaArquivoCorrompido( $preid ) ){
		return 'Prezada(o) Usuaria(o) - Esta obra contém arquivos corrompidos. Favor anexar novamente os documentos ou fotos anexados em vermelho clicando no botão \\\'Substituir\\\' , nas abas \\\'Cadastro de fotos do terreno\\\', \\\'Documentos Anexos\\\' e \\\'Projetos - Tipo A\\\', se houver. Posteriormente, tramitar a obra para analise do FNDE - Atenciosamente - Equipe SIMEC/PAR.';
	}

	//Carrega Dados Preenchimento
	$boPlanilhaOrcamentaria = $oSubacaoControle->verificaPlanilhaOrcamentaria($preid, SIS_OBRAS, $preid);
	$pacDados 				= $oSubacaoControle->verificaTipoObra($preid, SIS_OBRAS);
	$pacFNDE 				= $oSubacaoControle->verificaObraFNDE($preid, SIS_OBRAS);
	$pacFotos 				= $oSubacaoControle->verificaFotosObra($preid, SIS_OBRAS);
	$pacDocumentos 			= $oSubacaoControle->verificaDocumentosObra($preid, SIS_OBRAS, $pacDados);

	if($pacFNDE == 'f'){
		$pacDocumentosTipoA = $oSubacaoControle->verificaDocumentosObra($preid, SIS_OBRAS, $pacDados, true);
	}

	$pacQuestionario 		= $oPreObra->verificaQuestionario($qrpid);
	$pacCronograma 			= $oPreObra->verificaCronograma($preid);

	$boPlanilhaOrcamentaria['faltam'] = $boPlanilhaOrcamentaria['itcid'] - $boPlanilhaOrcamentaria['ppoid'];

	$arPendencias = array(
			'Dados do terreno' => 'Falta o preenchimento dos dados.',
			'Relatório de vistoria' => 'Falta o preenchimento dos dados.',
			'Cadastro de fotos do terreno' => 'Deve conter no mínimo 3 fotos do terreno.',
			'Cronograma físico-financeiro' => 'Falta o preenchimento dos dados.',
			'Documentos anexos' => 'Falta anexar os arquivos.',
			'Projetos - Tipo A' => 'Falta anexar os arquivos.',
			'Itens Planilha orçamentária' => 'Falta(m) '.$boPlanilhaOrcamentaria['faltam'].' iten(s) a ser(em) preenchido(s) na planilha orçamentaria.',
			'Planilha orçamentária' => 'Falta(m) '.$boPlanilhaOrcamentaria['faltam'].' iten(s) a ser(em) preenchido(s) na planilha orçamentaria.',
// 			'Valor da planilha orçamentária' => 'O valor {valor} não confere, deve estar entre R$ '.formata_valor($boPlanilhaOrcamentaria['minimo']).' e R$ '.formata_valor($boPlanilhaOrcamentaria['maximo']).'.'
			'Valor da planilha orçamentária' => 'O valor {valor} não confere, deve ser maior R$ '.formata_valor($boPlanilhaOrcamentaria['minimo']).'.'
	);


	$sql = "select ptoid from obras.pretipoobra where ptoprojetofnde = 'f' AND ptostatus = 'A'";
	$arrExcTipoObra = $db->carregarColuna( $sql );

	$msg = '';
	foreach($arPendencias as $k => $v){
		if(
			( !$pacDados && $k == 'Dados do terreno' ) ||
			( $k == 'Relatório de vistoria' && $pacQuestionario != 22 ) ||
			( $pacFotos < 3 && $k == 'Cadastro de fotos do terreno' ) ||
			( $k == 'Itens Planilha orçamentária' && $boPlanilhaOrcamentaria['faltam'] > 0 && !in_array($pacDados, $arrExcTipoObra)) ||
			( $k == 'Planilha orçamentária' && $boPlanilhaOrcamentaria['ppoid'] == 0 && $pacFNDE == 't') ||
			(
				$k == 'Valor da planilha orçamentária' &&
			  	(
			   		$boPlanilhaOrcamentaria['valor'] < $boPlanilhaOrcamentaria['minimo'] && $boPlanilhaOrcamentaria['minimo'] != ''/* ||
			   		$boPlanilhaOrcamentaria['valor'] > $boPlanilhaOrcamentaria['maximo'] && $boPlanilhaOrcamentaria['maximo'] != ''*/
			  	)
			) ||
			( $k == 'Cronograma físico-financeiro' && !in_array($ptoid, Array(73,74)) && !$pacCronograma && $pacFNDE == 't' ) ||
			( ($pacDocumentos['arqid'] < $pacDocumentos['podid'] || !$pacDocumentos) && $k == 'Documentos anexos' )
		){
			$msg .= $v.'\n';
		}
	}
	if( $msg != '' ){
		return $msg;
	}else{
		return true;
	}
}

function verificaEnvioAnaliseMIPraFrente($preid){
	global $db;

	$sql = "SELECT sfocontrapartidainformada > 0 as teste FROM par.solicitacaoreformulacaoobras p WHERE preid = $preid";
	$sfocontrapartidainformada = $db->pegaUm($sql);
	
	if( $sfocontrapartidainformada == 't' ){
		return 'Obras com contrapartida informada não podem ser tramitadas até a definição das regras';
	} else {
		return true;
	}
}

function WF_COND_reformulacao_enviar_analise( $preid ){

	global $db;

	// Verifica pendencias das obras
	$retornoVerificacaoPAR = verifica_pendencias_preObra( $preid );

	if( ($retornoVerificacaoPAR !== true) && ($retornoVerificacaoPAR !== false) )
	{
		return $retornoVerificacaoPAR;

	} elseif( $retornoVerificacaoPAR == false )
	{
		return false;
	}

	return true;
}

function WF_COND_diligencia_enviar_analise( $preid ){

	global $db;

	$obraEmenda = verificaObraEmenda($preid);
	// Essa função existia no workflow mas n está declarada nos arquivos .php
	// 	$retornoVerificacaoPAR = pendenciaArquivoWF( preid );

	$sql = "SELECT docid FROM obras.preobra WHERE preid = $preid";

	$docid = $db->pegaUm( $sql );

	$sql = "SELECT dioqtddia FROM par.diligenciaobra WHERE dioativo = true and preid = $preid";

	$diligencia = $db->pegaUm( $sql );

	if($diligencia){

		$dias = $diligencia.'DAYS';

		$sql = "SELECT
					(CAST(MAX(hst.htddata) AS date) + INTERVAL '".$dias."') < now() AS vencida
				FROM
					workflow.historicodocumento hst
				INNER JOIN workflow.acaoestadodoc aed ON aed.aedid = hst.aedid
				WHERE
					docid = {$docid}
					AND aed.esdiddestino = 328;";
	}else{

		$sql = "SELECT
					(CAST(MAX(hst.htddata) AS date) + INTERVAL '30DAYS') < now() AS vencida
				FROM
					workflow.historicodocumento hst
				INNER JOIN workflow.acaoestadodoc aed ON aed.aedid = hst.aedid
				WHERE
					docid = {$docid}
					AND aed.esdiddestino = 328;";
	}

	$obraVencida = $db->pegaUm( $sql );

	if( $obraVencida == 't' && !$obraEmenda ){
		return "Obra com diligência vencida.";
	}

	//Pega ano da obra
	$sql = "SELECT preano FROM obras.preobra WHERE preid = $preid";

	$preano = $db->pegaUm($sql);

	if($preano == false){
		return "Erro ao tentar carregar o ano da Obra ";
	}

    $possuiEmpenho = false;
    if($preid){
        $sql = "select count(*)
                from par.empenhoobrapar eo
                    inner join par.empenho e on e.empid = eo.empid and trim(e.empsituacao) != 'CANCELADO' and eobstatus = 'A' and empstatus = 'A'
                where preid = '{$preid}'";
        $possuiEmpenho = $db->pegaUm($sql);
    }

    // Verifica obras pendentes
    if(isset($_SESSION['par']['itrid'])){
        $possuiPendenciaObras = $_SESSION['par']['itrid'] == 2 ? getObrasPendentesPAR($_SESSION['par']['muncod']) : getObrasPendentesPAR(null, $_SESSION['par']['estuf']);

        if( !empty($possuiPendenciaObras) && !$possuiEmpenho && !$obraEmenda ){
            return "Existem obras com pendências no monitoramento de obras.";
        }
    }
/*
	// se for um desses anos, bloqueia caso haja pendência no obras 2.0
	$arrAno = Array(2011,2012,2013,2014);

	if( in_array($preano, $arrAno) ){
		$result = verificaPendenciasObras( $preid );
		if($result['erro'])
		{
			return $result['mensagem'];
		}
	}
*/
	// Verifica pendencias das obras
	$retornoVerificacaoPAR = verifica_pendencias_preObra( $preid );

	if( ($retornoVerificacaoPAR !== true) && ($retornoVerificacaoPAR !== false) )
	{
		return $retornoVerificacaoPAR;

	} elseif( $retornoVerificacaoPAR == false )
	{
		return false;
	}
	return true;
}

function WF_COND_cadastramento_enviar_analise( $preid ){

	global $db;

    $esfera = $db->pegaUm("select preesfera from obras.preobra where preid = '$preid'");
    $obraEmenda = verificaObraEmenda($preid);

    if(date('Ymd') >= '20150630' && $esfera == 'M' && !$obraEmenda){
        return "Prazo expirado.";
    }

	$sql = "SELECT preesfera, preano, tooid FROM obras.preobra WHERE preid = $preid";
	$dadosObra = $db->pegaLinha($sql);

	//Pega esfera da obra
	$preesfera = $dadosObra['preesfera'];

	//Pega ano da obra
	$preano = $dadosObra['preano'];

	//Pega a origem da obra
	$preano = $dadosObra['tooid'];

	if( $tooid != 1 ){
		$sql = "SELECT sobano
				FROM par.subacaoobra sbo
				INNER JOIN par.subacao sba ON sba.sbaid = sbo.sbaid
				WHERE
					preid = $preid
					AND sbastatus = 'A'";
		$preano = $db->pegaUm($sql);
	}

	if($preano == false){
		return "Erro ao tentar carregar o ano da Obra ";
	}

	// se for um desses anos bloqueia a ação para município
	$arrAnoBloqueia = Array(2011,2012,2013);

	if( in_array($preano, $arrAnoBloqueia) && $preesfera == 'M' ){
		return 'Prazo de envio para obras de 2011, 2012 e 2013 encerrado';
	}

    $possuiEmpenho = false;
    if($preid){
        $sql = "select count(*)
                from par.empenhoobrapar eo
                    inner join par.empenho e on e.empid = eo.empid and trim(e.empsituacao) != 'CANCELADO' and eobstatus = 'A' and empstatus = 'A'
                where preid = '{$preid}'";
        $possuiEmpenho = $db->pegaUm($sql);
    }

    // Verifica obras pendentes
    if(isset($_SESSION['par']['itrid'])){
        $possuiPendenciaObras = $_SESSION['par']['itrid'] == 2 ? getObrasPendentesPAR($_SESSION['par']['muncod']) : getObrasPendentesPAR(null, $_SESSION['par']['estuf']);
        if(!empty($possuiPendenciaObras) && !$possuiEmpenho && !$obraEmenda){
            return "Existem obras com pendências no monitoramento de obras.";
        }
    }
/*
	// se for 2014 e municipal, bloqueia caso haja pendência no obras 2.0
	// se for estadual e for um desses anos, bloqueia caso haja pendência no obras 2.0
	$arrAno = Array(2011,2012,2013,2014);
	if( $preano == 2014 || ( in_array($preano, $arrAno) && $preesfera == 'E' ) ){

		$result = verificaPendenciasObras( $preid );

		if($result['erro'])
		{
			return $result['mensagem'];
		}
	}
*/
	// Verifica pendencias das obras
	$retornoVerificacaoPAR = verifica_pendencias_preObra( $preid );

	if( ($retornoVerificacaoPAR !== true) && ($retornoVerificacaoPAR !== false) )
	{
		return $retornoVerificacaoPAR;

	} elseif( $retornoVerificacaoPAR == false )
	{
		return false;
	}


	return true;
}

function verificaWFpreObra($preid)
{
	global $db;

	/**
	 * Regra vpalida apenas para Obras do PAR
	 */

	$docid = prePegarDocid($preid);
	$esdid = prePegarEstadoAtual($docid);

	$descOrigemObra = retornaTipoOrigemObra($preid);

	if($esdid != WF_TIPO_EM_CORRECAO ){
		if($_SESSION['proinfancia2014']){
			return 'Prazo para o envio de projetos do Proinfância 2014 expirado';
		}
	}

	$arrEsdids = Array(WF_SUBACAO_ELABORACAO,WF_SUBACAO_DILIGENCIA,WF_SUBACAO_DILIGENCIA_CONDICIONAL);

	$sbaidTava = $_REQUEST['sbaid'];
	$anoTava = $_REQUEST['ano'];

	$arrTipoObras = $db->pegaLinha("select pre.tooid, pto.ptoclassificacaoobra, preanoselecao, pre.preesfera
									from obras.preobra pre
										inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
									where
										pre.preid = {$preid}
									    and pre.prestatus = 'A'
									    and pto.ptostatus = 'A'");
	$tooid = $arrTipoObras['tooid'];
	$preanoselecao = $arrTipoObras['preanoselecao'];

	// se for obra municipal, então trava
	$esfera = $db->pegaUm("SELECT preesfera FROM obras.preobra WHERE preid = ".$preid);
	if( $esfera ){
		$testeEsfera = $esfera == 'M' ? 2 : 1;
	} else {
		$testeEsfera = $_SESSION['par']['itrid'];
	}
	if($esdid != WF_TIPO_EM_REFORMULACAO && $esdid != WF_TIPO_EM_CORRECAO ){
		if($testeEsfera == 2 ){
		 	return false;
		}
	}

	if($sbaidTava == NULL || $sbaidTava == ''   ){

		$sbaidTava = $db->pegaUm("select sbaid
									from par.subacaoobra pre
									where
										pre.preid = {$preid}
									     limit 1 ");
	}

	if($anoTava == NULL || $anoTava == ''  ){
		$anoTava = $db->pegaUm("select sobano
									from par.subacaoobra pre
									where
										pre.preid = {$preid}
									     limit 1 ");
	}

	if($sbaidTava){
		if( verificaEstadoSubacao( $sbaidTava, $arrEsdids ) != 't' ){
			$testaPeriodo = bloqueioEnvioAnaliseObrasAtualizacao( $anoTava, $preid );

			if( !$testaPeriodo ){
				return "Não é possível enviar para análise. O prazo para atualização do PAR esgotou.";
			}
		}
	}

	$oPreObra = new PreObra();
	$oSubacaoControle = new SubacaoControle();

	if( !$_SESSION['par']['inuid'] ){
		$_SESSION['par']['inuid'] = $db->pegaUm( "SELECT
														CASE WHEN preesfera = 'E' THEN iu2.inuid ELSE iu.inuid END as local
													FROM
														obras.preobra pre
													LEFT JOIN par.instrumentounidade iu ON iu.muncod = pre.muncod
													LEFT JOIN par.instrumentounidade iu2 ON iu2.estuf = pre.estuf
													WHERE
														pre.preid = ".$preid );
	}


	$sql = "select liberatramitobra from par.instrumentounidade where inuid = ".$_SESSION['par']['inuid'];
	$valida = $db->pegaUm( $sql );


	// SE FOR OBRAS DO PAR
	 if( $tooid == 2  && $valida == 'f'  && !(in_array($esdid, array(WF_TIPO_EM_CORRECAO,WF_SUBACAO_DILIGENCIA, WF_SUBACAO_DILIGENCIA_CONDICIONAL)))){
	 	return false;
	 }


	 /* REGRA DE BLOQUEIO DO PAC 2
			- Proinfancia ano seleção anterior a 2014 Municipal -> envio bloqueado
	 */
	 // SE FOR OBRAS DO PAC
 	if( !in_array($esdid, array(WF_TIPO_EM_CORRECAO,WF_TIPO_EM_REFORMULACAO)) ){ // Se não for uma diligência ou reformulação ele verifica!
		if( ($tooid == 1 || ( $arrTipoObras['ptoclassificacaoobra'] == 'P' || $arrTipoObras['ptoclassificacaoobra'] == 'Q'))){
			if($arrTipoObras['preesfera'] == 'M' && $preanoselecao != '2014' ){
		 		return 'Envio bloqueado para Proinfancia com ano de seleção anterior a 2014.';
		 	}
		}
 	}
	/*
	if( $tooid == 1 && ( $arrTipoObras['ptoclassificacaoobra'] == 'P' || $arrTipoObras['ptoclassificacaoobra'] == 'Q'  ) && $preanoselecao != '2014' && !(in_array($esdid, array(WF_TIPO_EM_CORRECAO,WF_SUBACAO_DILIGENCIA, WF_SUBACAO_DILIGENCIA_CONDICIONAL))) ){
		return false;
	}
	*/

	if( $oPreObra->verificaArquivoCorrompido( $preid ) ){
		return 'Prezada(o) Usuaria(o) - Esta obra contém arquivos corrompidos. Favor anexar novamente os documentos ou fotos anexados em vermelho clicando no botão \\\'Substituir\\\' , nas abas \\\'Cadastro de fotos do terreno\\\', \\\'Documentos Anexos\\\' e \\\'Projetos - Tipo A\\\', se houver. Posteriormente, tramitar a obra para analise do FNDE - Atenciosamente - Equipe SIMEC/PAR.';
	}

	$qrpid = pegaQrpidPAC( $preid, 43 );

	//regras para a tramitação do wf
	$boPlanilhaOrcamentaria = $oSubacaoControle->verificaPlanilhaOrcamentaria($preid, SIS_OBRAS, $preid);
	$pacDados 				= $oSubacaoControle->verificaTipoObra($preid, SIS_OBRAS);
	$pacFNDE 				= $oSubacaoControle->verificaObraFNDE($preid, SIS_OBRAS);
	$pacFotos 				= $oSubacaoControle->verificaFotosObra($preid, SIS_OBRAS);
	$pacDocumentos 			= $oSubacaoControle->verificaDocumentosObra($preid, SIS_OBRAS, $pacDados);

	if($pacFNDE == 'f'){
		$pacDocumentosTipoA = $oSubacaoControle->verificaDocumentosObra($preid, SIS_OBRAS, $pacDados, true);
	}
	$pacQuestionario 		= $oPreObra->verificaQuestionario($qrpid);
	$pacLatitude	 		= $oPreObra->verificaLatitudePreObra($preid);
	$pacCronograma 			= $oPreObra->verificaCronograma($preid);
	$boPlanilhaOrcamentaria['faltam'] = $boPlanilhaOrcamentaria['itcid'] - $boPlanilhaOrcamentaria['ppoid'];

	$perfil = pegaArrayPerfil($_SESSION['usucpf']);

	$boDocumento = false;
	$boDocumentoTipoA = false;
	if($pacDocumentosTipoA['arqid'] == $pacDocumentosTipoA['podid']){
		$boDocumentoTipoA = true;
	}
	if($pacDocumentos['arqid'] == $pacDocumentos['podid']){
		$boDocumento = true;
	}

	$grupo = verificaGrupoMunicipio( $preid );

	$arTipoCodigo = array();

	if($esdid == WF_TIPO_EM_CADASTRAMENTO || $esdid == WF_TIPO_EM_CORRECAO){
		if( $grupo == TIPOMUN_GRUPO1_2011 ){
			$arTipoObra = Array(
								Array('codigo' => 1, 'descricao' => 'Escola Infantil - Tipo A' ),
								Array('codigo' => 38, 'descricao' => 'Escola Infantil - Tipo B (Projeto Novo)'),
								Array('codigo' => 39, 'descricao' => 'Escola Infantil - Tipo C (Projeto Novo)'),
								Array('codigo' => 43, 'descricao' => 'Escola Proinfância B - Metodologias Inovadoras'),
								Array('codigo' => 42, 'descricao' => 'Escola Proinfância C - Metodologias Inovadoras'),
								Array('codigo' => 44, 'descricao' => 'Escola Proinfância B - Metodologias Inovadoras - Emendas'),
								Array('codigo' => 45, 'descricao' => 'Escola Proinfância C - Metodologias Inovadoras - Emendas')
							);
			$arTipoCodigo = array(1, 38, 39, 43, 42, 44, 45);
		} else {
			$arTipoObra = Array(
								Array('codigo' => 38, 'descricao' => 'Escola Infantil - Tipo B (Projeto Novo)'),
								Array('codigo' => 39, 'descricao' => 'Escola Infantil - Tipo C (Projeto Novo)'),
								Array('codigo' => 43, 'descricao' => 'Escola Proinfância B - Metodologias Inovadoras'),
								Array('codigo' => 42, 'descricao' => 'Escola Proinfância C - Metodologias Inovadoras'),
								Array('codigo' => 43, 'descricao' => 'Escola Proinfância B - Metodologias Inovadoras - Emendas'),
								Array('codigo' => 42, 'descricao' => 'Escola Proinfância C - Metodologias Inovadoras - Emendas')
							);
			$arTipoCodigo = array(38, 39, 43, 42, 44, 45);
		}

		//$travaTipo = $db->pegaUm('SELECT \'N\' FROM obras.preobra WHERE premcmv IS TRUE AND preid = '.$preid);

		// Verifico se a Obra está nos tipos A, B, C ou nos projetos novos.
	//	if($travaTipo == 'N' && !in_array($pacDados, $arTipoObra)){
		if(!in_array($pacDados, $arTipoCodigo) && $arrTipoObras['ptoclassificacaoobra'] == 'P'){
			return 'A Obra não está nos tipos A, B, C ou nos projetos novos!';
		}
	}


	/*
	 * O bloco a seguir foi criado para tratar obras que não foram enviadas para analise juntamente com o par.
	 * Caso o perfil seja municipal, o par esteja em analise e a obra esteja preenchida então ele pode tramitar.
	 *
	 * Victor Benzi - 27/07/2012
	 */
	$validacao = true;

	$sql = "SELECT preid FROM par.subacaoobra WHERE preid = ".$preid;
	$so = $db->pegaUm($sql);
	if( $so ){ // Se é uma obra do par
		$sql = "select d.esdid from par.instrumentounidade iu inner join workflow.documento d ON d.docid = iu.docid where iu.inuid = ".$_SESSION['par']['inuid'];
		$estadoPar = $db->pegaUm( $sql );
		if( $estadoPar == WF_ANALISE ){ // Se o par estiver em analise e for uma equipe municipal então pode tramitar.
			if( in_array( PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil ) ||
				in_array( PAR_PERFIL_PREFEITO, $perfil ) ||
				in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) ||
				in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) ||
				in_array( PAR_PERFIL_EQUIPE_ESTADUAL, $perfil ) ||
				in_array( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil ) ||
				in_array( PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil ) ){ // Perfis Municipais
				$validacao = true;
			} else {
				$validacao = false;
			}
		}
	}
	/*
	 * Fim do bloco de alteração para a tramitação de obras com o PAR em analise.
	 */
	$testaGrupo = verificaGrupoMunicipio( $preid );

	if($validacao && $pacDados && $pacFotos >= 3 && $boDocumentoTipoA && $boDocumento && $pacQuestionario == 22 && $pacLatitude){
		if($pacFNDE == 't'){
			if( (str_replace(',','',number_format($boPlanilhaOrcamentaria['valor']) ) < $boPlanilhaOrcamentaria['minimo'] ||
			   str_replace(',','',number_format($boPlanilhaOrcamentaria['valor']) ) > $boPlanilhaOrcamentaria['maximo']) ||
			   $boPlanilhaOrcamentaria['faltam'] > 0 ||
			   $boPlanilhaOrcamentaria['ppoid'] != $boPlanilhaOrcamentaria['itcid'] || $pacCronograma == 0 ){
			   		if( $boPlanilhaOrcamentaria['minimo'] != '' && $boPlanilhaOrcamentaria['maximo'] != ''){
						return false;
			   		} else {
			   			return true;
			   		}
			}else{
				if( $testaGrupo != TIPOMUN_GRUPO3 ){
				//	if( ( date('YmdHis') <= DATA_EXPIRA_PROINFANCIA_PENDENCIAS_G_1_e_2 ) || $esdid == WF_TIPO_EM_CORRECAO){
						return true;
				/*	}else{
						if( $_SESSION['par']['inuid'] == 715 || $_SESSION['par']['inuid'] == 2824 ){ //Governador Valadares e Piauí (A pedido do Cabral e Thiago respectivamente)
							return true;
						} else {
							return 'A data para envio encerrou no dia 11/02/2011';
						}
					}*/
				}else{
			//		if( date('YmdHis') <= DATA_EXPIRA_PROINFANCIA_PENDENCIAS_G_3 || $esdid == WF_TIPO_EM_CORRECAO){
						return true;
			/*		}else{
						return "A data para envio encerrou no dia 15/04/2011";
					} */
				}
			}
		}else{
			if( $testaGrupo != TIPOMUN_GRUPO3 ){
		//		if( ( date('YmdHis') <= DATA_EXPIRA_PROINFANCIA_PENDENCIAS_G_1_e_2 ) || $esdid == WF_TIPO_EM_CORRECAO){
					return true;
		/*		}else{
					if( $_SESSION['par']['inuid'] == 715 || $_SESSION['par']['inuid'] == 2824 ){ //Governador Valadares e Piauí (A pedido do Cabral e Thiago respectivamente)
						return true;
					} else {
						return 'A data para envio encerrou no dia 11/02/2011';
					}
				} */
			}else{
			//	if( date('YmdHis') <= DATA_EXPIRA_PROINFANCIA_PENDENCIAS_G_3 || $esdid == WF_TIPO_EM_CORRECAO){
					return true;
			/*	}else{
					return "A data para envio encerrou no dia 15/04/2011";
				}*/
			}
		}
	}else{
		return false;
	}

}

function verificaArquivarWFpreObra($preid)
{
	$oPreObra = new PreObra();
	$oSubacaoControle = new SubacaoControle();

	if( !pendenciaArquivo( $preid ) ){
		return 'Prezada(o) Usuaria(o) - Esta obra contém arquivos corrompidos. Favor anexar novamente os documentos ou fotos anexados em vermelho clicando no botão \\\'Substituir\\\' , nas abas \\\'Cadastro de fotos do terreno\\\', \\\'Documentos Anexos\\\' e \\\'Projetos - Tipo A\\\', se houver. Posteriormente, tramitar a obra para analise do FNDE - Etenciosamente - Equipe SIMEC/PAR.';
	}
	return true;
}

function enviaObraAprovada($preid){

	global $db;

	//Se faz necessário explicação especifica. (Vitim ou Vitão)
	$sql = "SELECT
				aedid
			FROM workflow.acaoestadodoc
			WHERE
				esdiddestino = ".OBR_ESDID_EM_REFORMULACAO."
				AND esdidorigem = {$arObra['esdid']}";

	$aedid = $db->pegaUm($sql);

	if( $aedid == '' ){
		$sql = "INSERT INTO workflow.acaoestadodoc
					(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada,
					esdsncomentario, aedvisivel, aedcodicaonegativa)
				VALUES(
					{$arObra['esdid']}, ".OBR_ESDID_EM_REFORMULACAO.", 'Enviar para reformulação',
					'A', 'Enviada para reformulação',
					true, false, false )
				RETURNING
					aedid";

		$aedid = $db->pegaUm($sql);
	}

	wf_alterarEstado( $arObra['docid'], $aedid, 'Tramitado por reformularObra preid = '.$dados['preid'],
					  array( 'docid' => $arObra['docid'] ) );

// 	necessita depara stoid = esdid
//	$arObra = $db->pegaLinha("SELECT obrid, stoid FROM obr as.ob rainfraestrutura WHERE preid = ".$preid." and obsstatus = 'A'" );
//
//	$sql = "SELECT msoid, obrid, stoid, usucpf, msodtinclusao, msoorigem
//			FROM obras.movsituacaoobra
//			WHERE obrid = ".$arObra['obrid']." and msoorigem = 'P'";
//	$arrMovSituacao = $db->pegaLinha( $sql );
//	necessita depara stoid = esdid
//	$sql = "UPDATE obr as.ob rainfraestrutura SET stoid = '".$arrMovSituacao['stoid']."' WHERE preid = $preid and obsstatus = 'A'";
//	$db->executar($sql);
//
//	$sql = "INSERT INTO obras.movsituacaoobra(obrid, stoid, usucpf, msodtinclusao, msoorigem)
//			VALUES (".$arObra['obrid'].", ".$arObra['stoid'].", '".$_SESSION['usucpf']."', now(), 'P')";
//	$db->executar($sql);

	return $db->commit();
}
/*********************FIM************************/

/*function verificaInuidMunicipio($muncod)
{
	global $db;

	$sql = "select inuid from par.instrumentounidade where muncod = '{$muncod}'";

	//ver($db->pegaUm($sql),d);
	return $db->pegaUm($sql);
}

function verificaInuidEstado($estuf)
{
	global $db;

	$sql = "select inuid from par.instrumentounidade where estuf = '{$estuf}'";

	return $db->pegaUm($sql);
}*/

function verificaResponsabilidadeUsuarioPreObra($usucpf, $preid = null)
{
	global $db;

	$sql = "SELECT
				ur.pflcod,
				ur.estuf,
				ur.muncod
			FROM
				seguranca.usuario u
			INNER JOIN par.usuarioresponsabilidade ur ON ur.usucpf = u.usucpf
			WHERE
				u.usucpf = '{$usucpf}'";

	$arResponsabilidades = $db->carregar($sql);
	$arResponsabilidades = $arResponsabilidades ? $arResponsabilidades : array();

	foreach($arResponsabilidades as $dados){
		$arMuncod[] = $dados['muncod'];
	}
	$arMuncod = $arMuncod ? $arMuncod : array();
	$perfil = pegaPerfil($_SESSION['usucpf']);

	$perfis = array(PAR_PERFIL_SUPER_USUARIO, PAR_PERFIL_ADMINISTRADOR, PAR_PERFIL_EQUIPE_TECNICA);
	$preid = (int) $preid;
	if(!empty($preid)){

		$sql = "select muncod from obras.preobra where preid = '{$preid}'";
		$arPreobra = $db->pegaUm($sql);

		if(in_array($arPreobra, $arMuncod) || in_array($perfil, $perfis)){
			return true;
		}else{
			return false;
		}

	}else{

		return true;
	}

}

function verificaPermissãoEscritaUsuarioPreObra($usucpf, $preid = null)
{
	global $db;

	$sql = "SELECT
				ur.pflcod,
				ur.estuf,
				ur.muncod,
				ur.estuf
			FROM
				seguranca.usuario u
			INNER JOIN par.usuarioresponsabilidade ur ON ur.usucpf = u.usucpf
			WHERE
				u.usucpf = '{$usucpf}'";

	$arResponsabilidades = $db->carregar($sql);
	$arResponsabilidades = $arResponsabilidades ? $arResponsabilidades : array();

	foreach($arResponsabilidades as $dados){
		$arMuncod[] = $dados['muncod'];
		$arEstuf[] = $dados['estuf'];
	}

	$arMuncod = $arMuncod ? $arMuncod : array();
	$arEstuf = $arEstuf ? $arEstuf : array();

// 	$perfil = pegaPerfil($_SESSION['usucpf']); //Essa função pegaPerfil não funciona direito

	$perfis = array(PAR_PERFIL_SUPER_USUARIO, PAR_PERFIL_ADMINISTRADOR, PAR_PERFIL_COORDENADOR_GERAL);

	if($preid){
		$sql = "select muncodpar from obras.preobra where preid = '{$preid}'";
		$arPreobra = $db->pegaUm($sql);
		$sql = "select estufpar from obras.preobra where preid = '{$preid}' AND muncodpar IS NULL";
		$arPreobraUF = $db->pegaUm($sql);
		if(in_array($arPreobra, $arMuncod) || possuiPerfil($perfis) || in_array($arPreobraUF, $arEstuf)){
			return true;
		}else{
			return false;
		}

	}else{

		return true;
	}

}


function verificaQuestionarioQuestoes($qrpid)
{
	global $db;

	$sql = "SELECT count(1) FROM questionario.pergunta WHERE queid = ".QUESTOES_PONTUAIS_QUEID; //Pega o total de perguntas principal do questionário
	$totalPerguntaPrincipal = $db->pegaUm($sql);

	if($qrpid){
		$sql = "SELECT
					count (distinct p.perid)
					/*p.perid,
					qr.qrpid AS id,
					qr.queid,
					qr.qrpdata AS data_cadastro,
					p.pertitulo AS pergunta,
					ip.itptitulo AS resposta*/

				FROM
					par.parquestionario qq
				JOIN questionario.questionarioresposta qr USING (qrpid)
				JOIN questionario.questionario q ON q.queid = qr.queid
				JOIN questionario.pergunta p ON p.queid = q.queid
				JOIN questionario.itempergunta ip ON ip.perid = p.perid
				JOIN questionario.resposta r ON r.perid = p.perid
								AND r.qrpid = qr.qrpid
								AND r.itpid = ip.itpid
				WHERE
					qr.queid = ".QUESTOES_PONTUAIS_QUEID."
					AND qr.qrpid = $qrpid ";

		$totalPerguntaPrincipalRespondidas = $db->pegaUm($sql); //Pega o total de perguntas principais respondidas do questionário
	}
	if($totalPerguntaPrincipalRespondidas >= $totalPerguntaPrincipal){
		return true;
	}else{
		return false;
	}

	$countComSubPergunta = $countSemSubPergunta = 0;

	$sql = "SELECT
				perid,
				itpid
			FROM
				questionario.resposta
			WHERE
				perid in
					(SELECT
						perid
					 FROM
						questionario.pergunta
					 WHERE
						queid = ".QUESTOES_PONTUAIS_QUEID.")
				AND qrpid = $qrpid
			ORDER BY perid ";

	$arDados =  $db->carregar($sql);
	$arDados = ($arDados) ? $arDados : array();
	if(count($arDados) && $arDados[0]){
		if(count($arDados) < $totalPerguntaPrincipal){
			return false;
		}

		$totalResposta = 0;
		foreach ($arDados as $dados) {
			/**
			 * Bloco para tratar questão 9.
			 */
			if($dados['perid'] == 1523){
				if($dados['itpid'] != 2920 ){
					// Verifica se tem algum anexo para o item 9
					$sql = "SELECT
								count(qpa.arqid)
							FROM
								questionario.pergunta p
							INNER JOIN par.questoespontuaisanexos qpa ON p.perid = qpa.perid
							WHERE
								itpid in (SELECT
											itpid
										  FROM
										  	questionario.resposta r
								          WHERE
								          	r.perid IN (SELECT
								          					perid
								          				FROM
								          					questionario.pergunta
								                        WHERE
								                        	grpid IN (SELECT
								                        				grpid
								                        			  FROM
								                        			  	questionario.grupopergunta
								                                      WHERE
								                                      	itpid IN (SELECT
								                                      				itpid
								                                      			  FROM
								                                      			  	questionario.itempergunta
								                                                  WHERE
								                                                  	perid = 1523)))
								          AND r.qrpid = $qrpid
								          AND r.itpid IS NOT NULL)
								AND qpa.qrpid = $qrpid";
					$boAnexoPergunta = $db->pegaUm($sql);
					// 1528 é a pergunta 9.2, verificando se tem mais de uma escola
					$sql = "SELECT count(1) FROM par.questoespontuaisescolas WHERE perid = 1528 AND qrpid = $qrpid";
					$boEscola = $db->pegaUm($sql);
					if($boAnexoPergunta && $boEscola){
						$countComSubPergunta++;
					}
				}else{
						$countComSubPergunta++;
				}
			} else {
				$sql = "SELECT itpid FROM questionario.itempergunta WHERE perid = {$dados['perid']}";
				$arItemPergunta = $db->carregar($sql);
				$arItemPergunta = ($arItemPergunta) ? $arItemPergunta : array();
				if(count($arItemPergunta) && $arItemPergunta[0]){
					foreach($arItemPergunta as $itemPergunta){
						if($dados['itpid'] == $itemPergunta['itpid']){
						 	// verifica se a resposta da pergunta principal tem subpergunta
							$sql = "SELECT perid, pertipo FROM questionario.pergunta WHERE itpid = {$itemPergunta['itpid']} limit 1";
							$arSubPergunta = $db->pegaLinha($sql);
							$arSubPergunta = ($arSubPergunta) ? $arSubPergunta : array();
							if(count($arSubPergunta)){
								// verifica se a subpergunta foi respondida
								$sql = "SELECT resdsc, itpid FROM questionario.resposta WHERE perid = {$arSubPergunta['perid']} AND qrpid = $qrpid";
								$arRespostaSubPergunta = $db->carregar($sql);
								$arRespostaSubPergunta = ($arRespostaSubPergunta) ? $arRespostaSubPergunta : array();
								if(count($arRespostaSubPergunta) && $arRespostaSubPergunta[0]){
									// tratamos o array de resposta da subpergunta
									$arRespostaSubPerguntaTemp = array();
									foreach($arRespostaSubPergunta as $respostaSubPergunta){
										/**
										 * Bloco para tratar questão 4 e 11.
										 * Se for a resposta da questão 4 ou a 11 for a primeira opção, verifica outras subperguntas
										 */
										if($itemPergunta['itpid'] == 2882 || $itemPergunta['itpid'] == 2926){
											$sql = "SELECT perid, pertipo FROM questionario.pergunta WHERE itpid = {$respostaSubPergunta['itpid']} limit 1";
											$arSubPergunta2 = $db->pegaLinha($sql);
											$arSubPergunta2 = ($arSubPergunta2) ? $arSubPergunta2 : array();
											if(count($arSubPergunta2)){
												$sql = "SELECT resdsc, itpid FROM questionario.resposta where perid = {$arSubPergunta2['perid']} AND qrpid = $qrpid limit 1";
												$arRespostaSubPergunta2 = $db->pegalinha($sql);
												$arRespostaSubPergunta2 = ($arRespostaSubPergunta2) ? $arRespostaSubPergunta2 : array();
												if(count($arRespostaSubPergunta2)){
													$sql = "SELECT perid, pertipo FROM questionario.pergunta WHERE itpid = {$arRespostaSubPergunta2['itpid']} limit 1";
													$arSubPergunta3 = $db->pegaLinha($sql);
													$arSubPergunta3 = ($arSubPergunta3) ? $arSubPergunta3 : array();
													if(count($arSubPergunta3)){
														$sql = "SELECT count(arqid) FROM par.questoespontuaisanexos WHERE perid = {$arSubPergunta3['perid']} AND qrpid = $qrpid";
														$respostaAnexo = $db->pegaUm($sql);
														if($respostaAnexo){
															$countComSubPergunta++;
														}
													}
												}
											}
											continue;
										}
										/**
										 * Fim Bloco para tratar questão 4 e 11.
										 */

										// verifica se tem subpergunta. para depois verificarmos se tem anexo.
										if($respostaSubPergunta['itpid']){
											$sql = "SELECT count(1) FROM questionario.pergunta WHERE itpid = {$respostaSubPergunta['itpid']} ";
											$boSubPerguntaAnexo = $db->pegaUm($sql);
										}
										if($boSubPerguntaAnexo && $respostaSubPergunta['itpid']){
											$arRespostaSubPerguntaTemp[] = $respostaSubPergunta['itpid'];
										} elseif($arSubPergunta['pertipo'] == 'TX') {
											if(trim($respostaSubPergunta['resdsc']) && !empty($respostaSubPergunta['resdsc'])){
												$countComSubPergunta++;
											}
										}else{
											$countComSubPergunta++;
										}
									}

									// Se for a resposta da questão 4 ou a 11 for a primeira opção, pula o resto do código
									if($itemPergunta['itpid'] == 2882 || $itemPergunta['itpid'] == 2926){
										continue;
									}

									if(count($arRespostaSubPerguntaTemp)){
										// verificamos se alguma das respostas tem pergunta/anexo
										$sql = "SELECT
													count(arqid)
												FROM
													questionario.pergunta p
												INNER JOIN par.questoespontuaisanexos qpa on p.perid = qpa.perid
												WHERE
													qrpid = $qrpid
													AND itpid in ('". implode("','", $arRespostaSubPerguntaTemp)."')";
										$respostaAnexo = $db->pegaUm($sql);
									}
									if($respostaAnexo){
										$countComSubPergunta++;
									}

								}
							} else {
								$countSemSubPergunta++;
							}
						}
					} // fim foreach $arItemPergunta
				} // fim if $arItemPergunta
			} // fim do if do item 9
		} // fim foreach $arDados
	} // fim if $arDados

	//ver($countComSubPergunta, $countSemSubPergunta,$totalPerguntaPrincipal,d);
	if(($countComSubPergunta+$countSemSubPergunta) >= $totalPerguntaPrincipal ){
		return true;
	}

	return false;

}


function verificaCondicaoIndeferimento( $qrpid )
{
	global $db;

	$sql = "SELECT
				pre.preid
			FROM obras.preobra pre
			INNER JOIN  obras.preobraanalise poa ON poa.preid = pre.preid
			WHERE
				poa.qrpid = {$qrpid}";

	$preid = $db->pegaUm($sql);
	$preid = $preid ? $preid : $_SESSION['par']['preid'];
	if( !pendenciaArquivo( $preid ) ){
		return 'Prezada(o) Usuaria(o) - Esta obra contém arquivos corrompidos. Favor anexar novamente os documentos ou fotos anexados em vermelho clicando no botão \\\'Substituir\\\' , nas abas \\\'Cadastro de fotos do terreno\\\', \\\'Documentos Anexos\\\' e \\\'Projetos - Tipo A\\\', se houver. Posteriormente, tramitar a obra para analise do FNDE - Atenciosamente - Equipe SIMEC/PAR.';
	}

	$sql = "SELECT
				doc.esdid
			FROM obras.preobra pre
			INNER JOIN  obras.preobraanalise poa ON poa.preid = pre.preid
			INNER JOIN workflow.documento    doc ON pre.docid = doc.docid
			WHERE
				poa.qrpid = {$qrpid}
				AND doc.esdid IN (218, 336)";

	$esdid = $db->pegaUm($sql);

	$sql = "SELECT
				itptitulo
			FROM questionario.resposta r
			INNER JOIN questionario.itempergunta it ON it.itpid = r.itpid
			INNER JOIN questionario.pergunta      p ON p.perid  = it.perid
			WHERE
				r.itpid IN (SELECT
								itpid
							FROM questionario.itempergunta
							WHERE
								perid IN (SELECT
											perid
										  FROM
										  	questionario.pergunta
										  WHERE
										  	grpid IN (SELECT
										  				grpid
										  			  FROM
										  			  	questionario.grupopergunta
										  			  WHERE
										  			  	queid = 49 )))
				AND qrpid = {$qrpid}
				AND p.perid = 1301";
//				"
//	AND p.perid = ".QUESTAO_DOCUMENTO2;

	$resposta = $db->pegaUm($sql);

	if($resposta == 'Não.'){
		return true;
	}else{

		if($esdid){
			return true;
		}else{
			return false;
		}
	}
}

function verificaCondicaoDeferimento( $qrpid ){

	global $db, $preid;

	$sql = "SELECT
				pre.preid
			FROM obras.preobra pre
			INNER JOIN  obras.preobraanalise poa ON poa.preid = pre.preid
			WHERE
				poa.qrpid = {$qrpid}";

	$preid = $db->pegaUm($sql);
	$preid = $preid ? $preid : $_SESSION['par']['preid'];
	if( !pendenciaArquivo( $preid ) ){
		return 'Prezada(o) Usuaria(o) - Esta obra contém arquivos corrompidos. Favor anexar novamente os documentos ou fotos anexados em vermelho clicando no botão \\\'Substituir\\\' , nas abas \\\'Cadastro de fotos do terreno\\\', \\\'Documentos Anexos\\\' e \\\'Projetos - Tipo A\\\', se houver. Posteriormente, tramitar a obra para analise do FNDE - Atenciosamente - Equipe SIMEC/PAR.';
	}

	$sql = "SELECT
				itptitulo,
				pertitulo
			FROM
				questionario.resposta r
			INNER JOIN questionario.itempergunta it ON it.itpid = r.itpid
			INNER JOIN questionario.pergunta      p ON p.perid  = it.perid
			WHERE
				r.itpid IN (SELECT
								itpid
							FROM
								questionario.itempergunta
							WHERE perid IN (SELECT
												perid
											FROM
												questionario.pergunta
											WHERE
												grpid IN (SELECT
															grpid
														  FROM
														  	questionario.grupopergunta
														  WHERE
														  	queid = 49 )))
				AND qrpid = {$qrpid}";

	$respostas = $db->carregar($sql);

	if( count($respostas) == 13 )
	{
		for($i=0; $i<count($respostas); $i++)
		{
			if( $respostas[$i]['itptitulo'] == 'Não.' )
			{
				$retorno = false;
				if( substr($respostas[$i]['pertitulo'],0,3) == '1.2' )
				{
                                    return 'Não é possível enviar para validação de deferimento pois o terreno não é adequado e livre de impedimentos técnicos para a construção da obra pleiteada. Conforme questão 1.2 ';
				}
			}else {
                            $retorno = true;
				if( substr($respostas[$i]['pertitulo'],0,3) == '1.2' )
				{
                                    return $retorno;
				}
                        }
		}
	} else {
            return "É necessário preencher todos as questões do formulário para prosseguir.";
        }
}

function verificaCondicaoDiligencia( $qrpid ){

	global $db;

	$sql = "SELECT
				pre.preid
			FROM obras.preobra pre
			INNER JOIN  obras.preobraanalise poa ON poa.preid = pre.preid
			WHERE
				poa.qrpid = {$qrpid}";

	$preid = $db->pegaUm($sql);
	$preid = $preid ? $preid : $_SESSION['par']['preid'];
	if( !pendenciaArquivo( $preid ) ){
		return 'Prezada(o) Usuaria(o) - Esta obra contém arquivos corrompidos. Favor anexar novamente os documentos ou fotos anexados em vermelho clicando no botão \\\'Substituir\\\' , nas abas \\\'Cadastro de fotos do terreno\\\', \\\'Documentos Anexos\\\' e \\\'Projetos - Tipo A\\\', se houver. Posteriormente, tramitar a obra para analise do FNDE - Atenciosamente - Equipe SIMEC/PAR.';
	}

	$sql = "SELECT
				itptitulo,
				pertitulo
			FROM
				questionario.resposta r
			INNER JOIN questionario.itempergunta it ON it.itpid = r.itpid
			INNER JOIN questionario.pergunta      p ON p.perid  = it.perid
			WHERE
				r.itpid IN (SELECT
								itpid
							FROM
								questionario.itempergunta
							WHERE
								perid IN (SELECT
											perid
										  FROM
										 	questionario.pergunta
										  WHERE
										  	grpid IN (SELECT
										  				grpid
										  			  FROM
										  			  	questionario.grupopergunta
										  			  WHERE
										  			  	queid = 49 )))
				AND qrpid = {$qrpid}";

	$respostas = $db->carregar($sql);

	$retorno = false;
	if( count($respostas) == 13 )
	{
		for($i=0; $i<count($respostas); $i++)
		{
			if( $respostas[$i]['itptitulo'] == 'Não.' )
			{
				$retorno = true;
				if( substr($respostas[$i]['pertitulo'],0,3) == '1.2' )
				{
                                    return $retorno;
				}
			}else {
                            $retorno = false;
				if( substr($respostas[$i]['pertitulo'],0,3) == '1.2' )
				{
                                    return 'Não é possível enviar para diligência pois o terreno é adequado e livre de impedimentos técnicos para a construção da obra pleiteada. Conforme questão 1.2';
				}
                        }
		}
	} else {
            return "É necessário preencher todos as questões do formulário para prosseguir.";
        }

	return $retorno;
}

function verificaCondicaoIndeferimentoInicial( $preid ){
	global $db;

	if( $_SESSION['usucpf'] == '' ){
		return true;
	}

	if( !pendenciaArquivo( $preid ) ){
		return 'Prezada(o) Usuaria(o) - Esta obra contém arquivos corrompidos. Favor anexar novamente os documentos ou fotos anexados em vermelho clicando no botão \\\'Substituir\\\' , nas abas \\\'Cadastro de fotos do terreno\\\', \\\'Documentos Anexos\\\' e \\\'Projetos - Tipo A\\\', se houver. Posteriormente, tramitar a obra para analise do FNDE - Atenciosamente - Equipe SIMEC/PAR.';
	}

	$sql = "SELECT
				poaindeferido
			FROM obras.preobraanalise
			WHERE
				preid = {$preid}";

	$resposta =  $db->pegaUm($sql);
	if( $resposta == 'N' ){
		return true;
	} else {
		return false;
	}
}

function verificaCondicaoAnaliseInicial( $preid ){
	global $db;

	if( !pendenciaArquivo( $preid ) ){
		return 'Prezada(o) Usuaria(o) - Esta obra contém arquivos corrompidos. Favor anexar novamente os documentos ou fotos anexados em vermelho clicando no botão \\\'Substituir\\\' , nas abas \\\'Cadastro de fotos do terreno\\\', \\\'Documentos Anexos\\\' e \\\'Projetos - Tipo A\\\', se houver. Posteriormente, tramitar a obra para analise do FNDE - Atenciosamente - Equipe SIMEC/PAR.';
	}

	$sql = "SELECT
				poaindeferido
			FROM obras.preobraanalise
			WHERE
				preid = {$preid}";

	$resposta =  $db->pegaUm($sql);
	if( $resposta == 'S' ){
		return true;
	} else {
		return false;
	}
}
function calculaPrazoDiligenciaPAR( $preid ){
	global $db;

	$docid = prePegarDocid($preid);

	$sql = "SELECT
				CAST(max(hd.htddata) as date) + INTERVAL '30 DAYS' as data
			--	max(hd.htddata)
			FROM workflow.historicodocumento hd
			WHERE
				docid = {$docid}
				AND aedid = 872";

	$data = $db->pegaUm($sql);

	$arData = explode(" ", $data);

	$grupo = verificaGrupoMunicipio($preid);

	if(!empty($arData[0])){
		if( in_array($grupo,Array(TIPOMUN_GRUPO1,TIPOMUN_GRUPO2,TIPOMUN_GRUPO1_2011,TIPOMUN_GRUPO2_2011))){
			return $arData[0];
			//return somar_dias_uteis($arData[0], 11);
		}elseif( $grupo == TIPOMUN_GRUPO3 || $grupo == TIPOMUN_GRUPO3_2011 ){
			return $arData[0];
			//return somar_dias_uteis($arData[0], 11);
		}else{
			return false;
		}
	}
}
function calculaPrazoDiligencia( $preid ){

	global $db;

	$docid = prePegarDocid($preid);

	$sql = "SELECT
				CAST(max(hd.htddata) as date) + INTERVAL '30 DAYS' as data
			--	max(hd.htddata)
			FROM workflow.historicodocumento hd
			WHERE
				docid = {$docid}
				AND aedid = 516";

	$data = $db->pegaUm($sql);

	$arData = explode(" ", $data);

	$grupo = verificaGrupoMunicipio($preid);

	if(!empty($arData[0])){
		if( in_array($grupo,Array(TIPOMUN_GRUPO1,TIPOMUN_GRUPO2,TIPOMUN_GRUPO1_2011,TIPOMUN_GRUPO2_2011))){
			return $arData[0];
			//return somar_dias_uteis($arData[0], 11);
		}elseif( $grupo == TIPOMUN_GRUPO3 || $grupo == TIPOMUN_GRUPO3_2011 ){
			return $arData[0];
			//return somar_dias_uteis($arData[0], 11);
		}else{
			return false;
		}
	}
}
function enviaEmailDiligenciaPAR( $preid ){

	global $db;

	$dataFinal = calculaPrazoDiligenciaPAR( $preid );

	$arData = explode("-", $dataFinal);

	$sql1 = "SELECT
				mun.muncod,
				pre.predescricao,
				pto.ptodescricao,
				mun.mundescricao,
				mun.estuf
			FROM
				obras.preobra pre
			INNER JOIN obras.pretipoobra     pto ON pto.ptoid  = pre.ptoid
			INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod
			WHERE
				pre.preid = {$preid}";

	$arDadosObra = $db->pegaLinha($sql1);

	if($_SESSION['par']['esfera']=='M'){
		$sql2 = "SELECT
					usunome,
					usuemail
				 FROM
				 	seguranca.perfil perfil
				 INNER JOIN seguranca.perfilusuario    perfilusuario ON perfil.pflcod  = perfilusuario.pflcod
				 													 	AND perfil.pflcod = ".PAR_PERFIL_EQUIPE_MUNICIPAL."
				 INNER JOIN seguranca.usuario                usuario ON usuario.usucpf = perfilusuario.usucpf
				 INNER JOIN seguranca.usuario_sistema usuariosistema ON usuario.usucpf = usuariosistema.usucpf
				 WHERE
				 	usunome IS NOT NULL
				 	AND usuariosistema.suscod = 'A'
				 	AND usuariosistema.sisid = '23'
				 	AND usuario.regcod = '{$arDadosObra['estuf']}'
				 	AND usuario.muncod = '{$arDadosObra['muncod']}'
				 	AND (perfil.pflcod = ".PAR_PERFIL_EQUIPE_MUNICIPAL." OR usuariosistema.pflcod = ".PAR_PERFIL_EQUIPE_MUNICIPAL.")";
	}else{
		$sql2 = "SELECT
					entemail as usuemail
				FROM
					par.entidade ent
				WHERE
					ent.estuf = '{$arDadosObra['estuf']}'
					AND dutid = ".DUTID_SECRETARIA_ESTADUAL;
	}

	$arDadosUsuarios = $db->carregar($sql2);
	$arDadosUsuarios = $arDadosUsuarios ? $arDadosUsuarios : array();

	//atualiza dias da diligencia!
	$sql = "UPDATE par.diligenciaobra SET diodata=NOW() WHERE diodata IS NULL and dioativo = true AND preid = ".$preid;
	$db->executar($sql);
	$db->commit();

	$sql = "SELECT dioqtddia, diodata FROM par.diligenciaobra WHERE dioativo = 't' and dioativo = true AND preid = ".$preid;
	$dados = $db->pegaLinha( $sql );

	if(is_array($dados) && $dados['diodata'] && $dados['dioqtddia']) {
		$arData = explode('-', $dados['diodata']);

		$ano = $arData[0];
		$mes = $arData[1];
		$dia = $arData[2];
		$dataFinal = mktime(24*$dados['dioqtddia'], 0, 0, $mes, $dia, $ano);
		$dataFormatada = date('Y-m-d',$dataFinal);

		$arData2 = explode('-', $dataFormatada);
		$ano = $arData2[2];
		$mes = $arData2[1];
		$dia = $arData2[0];
	} else {
		$ano = $arData[0];
		$mes = $arData[1];
		$dia = $arData[2];
	}

	$remetente = array('nome'=>$_SESSION['usunome'], 'email'=>$_SESSION['email_sistema']);

	$assunto  = "Em diligência - {$arDadosObra['predescricao']} - ". SIGLA_SISTEMA;

	$conteudo = "<p>Senhor(a) Dirigente Municipal,</p>

				<p>O seu município cadastrou e enviou o(s) seguinte(s) projeto(s) de infraestrutura referente(s) ao PAR pelo Simec - Módulo PAR:</p>

				Nome da Obra: {$arDadosObra['predescricao']}<br/>
				Tipo da Obra: {$arDadosObra['ptodescricao']}<br/>
				Nº identificação: {$preid}<br/>

				<p>Após a análise do FNDE nº {$preid}, verificamos que a proposta(s) está na situação \"em diligência\".</p>

				<p>Solicitamos que a equipe municipal acesse o PAR, clique na obra que está em diligência e, depois, na aba \"Análise de Engenharia\" (abrir todos).</p>

				<p>Nos itens da análise de engenharia em que a resposta é \"não\", deve-se ler a \"Observação\", ajustar o que é solicitado e tramitar para nova análise até às 23 horas e 59 minutos do dia {$dia} de ".mes_extenso($mes)." de {$ano}. Após esta data o sistema será fechado para recebimento de resposta das diligências, resultando no indeferimento da ação nesta etapa do PAR.</p>

				<p>Caso o município tenha outro(s) projeto(s) que se encontra(m) na situação \"Aguardando análise - FNDE\", a equipe municipal deve acompanhar a situação. Se essa(s) obra(s) entrar(em) \"em diligência\", o mesmo procedimento deve ser seguido.</p>


				<p>Atenciosamente,</p>


				<p>Equipe Técnica do PAR</p>";

	if (!IS_PRODUCAO) {
// 		enviar_email($remetente, '', $assunto, $conteudo, $cc, $cco );
	} else {
		foreach($arDadosUsuarios as $dados) {
			enviar_email(array('nome'=>SIGLA_SISTEMA. ' - PAR', 'email'=>'noreply@mec.gov.br'), $dados['usuemail'], $assunto, $conteudo, $cc, $cco );
		}
	}

	return true;

}

function enviaObraParaDiligencia($preid){
	global $db;

	$sql = "SELECT tooid FROM obras.preobra WHERE preid = ".$preid;
	$tipoObra =  $db->pegaUm($sql);

	if($tipoObra == 1){
		$origemObra = 'PAC';
	}else{
		$origemObra = 'PAR';
	}

	$sql = "SELECT count(*) FROM par.diligenciaobra WHERE dioativo = true and preid =".$preid;
	$temDiligencia =  $db->pegaUm($sql);

	if($temDiligencia){
		//$sql = "UPDATE par.diligenciaobra SET diodata = NOW(), dioqtddia = 30 WHERE dioativo = true and preid = {$preid}";
	}else{
		$sql = "INSERT INTO par.diligenciaobra ( diodata, diotipo, dioqtddia, preid, dioativo  ) VALUES ( NOW(), '{$origemObra}', 30, {$preid}, true); ";
	}

	if($db->executar($sql)){
		$db->commit();
		enviaEmailDiligenciaProinfancia( $preid );

		return true;
	}else{
		return false;
	}
}

function enviaEmailDiligenciaProinfancia( $preid ){

	global $db;

//	$docid = prePegarDocid($preid);
//
//	$sql = "SELECT
//				max(hd.htddata)
//			FROM workflow.historicodocumento hd
//			WHERE
//				docid = {$docid}
//				AND aedid = 516";
//
//	$data = $db->pegaUm($sql);
//
//	$arDataHora = explode(" ", $data);

//	$grupo = verificaGrupoMunicipio($preid);
//	if(!empty($arDataHora[0])){
//		if( in_array($grupo,Array(TIPOMUN_GRUPO1,TIPOMUN_GRUPO2))){
//			$dataFinal = somar_dias_uteis($arDataHora[0], 11);
//		}elseif( $grupo == TIPOMUN_GRUPO3 ){
//			$dataFinal = somar_dias_uteis($arDataHora[0], 11);
//		}else{
//			return false;
//		}
//	}

	$dataFinal = calculaPrazoDiligencia( $preid );

	$arData = explode("-", $dataFinal);

	$sql1 = "SELECT
				mun.muncod,
				pre.predescricao,
				pto.ptodescricao,
				mun.mundescricao,
				mun.estuf
			FROM
				obras.preobra pre
			INNER JOIN obras.pretipoobra     pto ON pto.ptoid  = pre.ptoid
			INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod
			WHERE
				pre.preid = {$preid}";

	$arDadosObra = $db->pegaLinha($sql1);

	if($_SESSION['par']['esfera']=='M'){
		$dirigente = "Municipal";
		$sql2 = "SELECT
					usunome,
					usuemail
				 FROM
				 	seguranca.perfil perfil
				 INNER JOIN seguranca.perfilusuario    perfilusuario ON perfil.pflcod  = perfilusuario.pflcod
				 													 	AND perfil.pflcod = ".PAR_PERFIL_EQUIPE_MUNICIPAL."
				 INNER JOIN seguranca.usuario                usuario ON usuario.usucpf = perfilusuario.usucpf
				 INNER JOIN seguranca.usuario_sistema usuariosistema ON usuario.usucpf = usuariosistema.usucpf
				 WHERE
				 	usunome IS NOT NULL
				 	AND usuariosistema.suscod = 'A'
				 	AND usuariosistema.sisid = '23'
				 	AND usuario.regcod = '{$arDadosObra['estuf']}'
				 	AND usuario.muncod = '{$arDadosObra['muncod']}'
				 	AND (perfil.pflcod = ".PAR_PERFIL_EQUIPE_MUNICIPAL." OR usuariosistema.pflcod = ".PAR_PERFIL_EQUIPE_MUNICIPAL.")";
	}else{
		$dirigente = "Estadual";
		$sql2 = "SELECT
					entemail as usuemail
				FROM
					par.entidade ent
				WHERE
					ent.estuf = '{$arDadosObra['estuf']}'
					AND dutid = ".DUTID_SECRETARIA_ESTADUAL;
	}

	$arDadosUsuarios = $db->carregar($sql2);
	$arDadosUsuarios = $arDadosUsuarios ? $arDadosUsuarios : array();

	//atualiza dias da diligencia!
	//$sql = "UPDATE par.diligenciaobra SET diodata=NOW() WHERE diodata IS NULL AND preid = ".$preid;
	//$db->executar($sql);
	//$db->commit();

	$sql = "SELECT dioqtddia, diodata FROM par.diligenciaobra WHERE dioativo = 't' AND preid = ".$preid;
	$dados = $db->pegaLinha( $sql );

	if(is_array($dados) && $dados['diodata'] && $dados['dioqtddia']) {
		$arData = explode('-', $dados['diodata']);

		$ano = $arData[0];
		$mes = $arData[1];
		$dia = $arData[2];
		$dataFinal = mktime(24*$dados['dioqtddia'], 0, 0, $mes, $dia, $ano);
		$dataFormatada = date('Y-m-d',$dataFinal);

		$arData2 = explode('-', $dataFormatada);
		$ano = $arData2[2];
		$mes = $arData2[1];
		$dia = $arData2[0];
	} else {
		$ano = $arData[0];
		$mes = $arData[1];
		$dia = $arData[2];
	}

	$remetente = array('nome'=>$_SESSION['usunome'], 'email'=>$_SESSION['email_sistema']);

	$assunto  = "Em diligência - {$arDadosObra['predescricao']} - ". SIGLA_SISTEMA;

	$conteudo = "<p>Senhor(a) Dirigente $dirigente,</p>

				<p>O seu município cadastrou e enviou para analise o(s) seguinte(s) projeto(s) de infraestrutura referente(s) ao PAC 2 (Proinfância e/ou construção de quadras cobertas) pelo Simec - Módulo PAR:</p>

				Nome da Obra: {$arDadosObra['predescricao']}<br/>
				Tipo da Obra: {$arDadosObra['ptodescricao']}<br/>
				Nº identificação: {$preid}<br/>

				<p>Após a análise do FNDE nº {$preid}, verificamos que a proposta(s) está na situação \"em diligência\",
				necessitando que a Prefeitura realize correções e complementações nos documentos.</p>

				<p>Solicitamos que a equipe municipal acesse o SIMEC, módulo PAR 2010, abra a lista de obras, clique na ação
				que está na situação \"Em diligência\" e, depois, na aba \"Análise de Engenharia\".A análise de engenharia
				possui 12 perguntas, que foram respondidas pela equipe do FNDE, sobre cada documento enviado. Todas
				as perguntas que possuírem a resposta \"não\", apresentarão as pendências descritas no campo
				\"Observação\". A equipe municipal deve ler todas as perguntas e, para isso, poderá clicar no ícone chamado
				de \"Próximo\".</p>

				<p>O interessado deve corrigir todas as pendências nas demais abas, ao lado da aba \"Análise de engenharia\", e
				enviar a ação para análise novamente até às 23 horas e 59 minutos do dia {$ano} de ".mes_extenso($mes)." de {$dia}.
				Após esta data o sistema será fechado para recebimento de resposta das diligências, resultando no indeferimento da
				ação nesta etapa do PAC 2.</p>

				<p>Caso o município tenha outro(s) projeto(s) que se encontra(m) na situação em análise, a equipe
				municipal deve acompanhar a situação. Se essa(s) obra(s) entrar(em) \"em diligência\", o mesmo procedimento deve ser seguido.</p>


				<p>Atenciosamente,</p>


				<p>Equipe Técnica do PAR</p>";

/*
	$conteudo = "<p>Senhor(a) Dirigente Municipal,</p>

				<p>O seu município cadastrou e enviou o(s) seguinte(s) projeto(s) de infraestrutura referente(s) ao PAC 2 (Proinfância e/ou construção de quadras cobertas) pelo Simec - Módulo PAR:</p>

				Nome da Obra: {$arDadosObra['predescricao']}<br/>
				Tipo da Obra: {$arDadosObra['ptodescricao']}<br/>
				Nº identificação: {$preid}<br/>

				<p>Após a análise do FNDE nº {$preid}, verificamos que a proposta(s) está na situação \"em diligência\".</p>

				<p>Solicitamos que a equipe municipal acesse o PAR 2010, clique na obra que está em diligência e, depois, na aba \"Análise de Engenharia\" (abrir todos).</p>

				<p>Nos itens da análise de engenharia em que a resposta é \"não\", deve-se ler a \"Observação\", ajustar o que é solicitado e tramitar para nova análise até às 23 horas e 59 minutos do dia 31 de Dezembro de 2011. Após esta data o sistema será fechado para recebimento de resposta das diligências, resultando no indeferimento da ação nesta etapa do PAC 2.</p>

				<p>Caso o município tenha outro(s) projeto(s) que se encontra(m) na situação \"Aguardando análise - FNDE\", a equipe municipal deve acompanhar a situação. Se essa(s) obra(s) entrar(em) \"em diligência\", o mesmo procedimento deve ser seguido.</p>


				<p>Atenciosamente,</p>


				<p>Equipe Técnica do PAR</p>";
*/

	if (!IS_PRODUCAO) {
// 		enviar_email($remetente, '', $assunto, $conteudo, $cc, $cco );
	} else {
		foreach($arDadosUsuarios as $dados){

			enviar_email(array('nome'=>SIGLA_SISTEMA. ' - PAR', 'email'=>'noreply@mec.gov.br'), $dados['usuemail'], $assunto, $conteudo, $cc, $cco );
		}

		$arCopias = array($_SESSION['email_sistema']);

		foreach($arCopias as $dados){

			enviar_email(array('nome'=>SIGLA_SISTEMA. ' - PAR', 'email'=>'noreply@mec.gov.br'), $dados, $assunto, $conteudo, $cc, $cco );

		}
	}

	return true;

}

function atualizaEstadoDiligenciaWF($preid){

	global $db;

	$oPreObra = new PreObra();
	$oPreObra->atualizaCPFUltimoEnvioValidacao( $_SESSION['usucpf'], $preid );

	$sql = "UPDATE par.diligenciaobra SET dioativo='f' WHERE dioativo='t' AND preid = ".$preid;
	$db->executar($sql);
	$db->commit();

	return true;
}

function wf_pos_validacao_diligencia( $preid ){

	$oPreObra = new PreObra();
	$oPreObra->atualizaCPFUltimoEnvioValidacao( $_SESSION['usucpf'], $preid );

	return true;
}

function wf_pos_validacao_indeferimento( $preid ){

	$oPreObra = new PreObra();
	$oPreObra->atualizaCPFUltimoEnvioValidacao( $_SESSION['usucpf'], $preid );

	return true;
}

function wf_pos_validacao_deferimento( $preid ){

	$oPreObra = new PreObra();
	$oPreObra->atualizaCPFUltimoEnvioValidacao( $_SESSION['usucpf'], $preid );

	return true;
}

function reenviaEmailDiligenciaProinfancia( $grupo ){

	global $db;

	$sql = "SELECT
				preid
			FROM
				obras.preobra po
			INNER JOIN territorios.muntipomunicipio tmp ON tmp.muncod = po.muncod
			WHERE
				tmp.tpmid in (".$grupo.")";
	$preids = $db->carregarColuna($sql);
//	$x=1;
	foreach( $preids as $preid ){
//		$x++;
		enviaEmailDiligenciaProinfancia( $preid );
	}
//	echo "<script>alert('Enviou para {$x} preobras');</script>";
}

function verificaPeriodoEnviarDiligencia( $preid )
{
	global $db;

	$docid = prePegarDocid($preid);
	$esdid = prePegarEstadoAtual($docid);

	$retornoVerificacao = verificaWFpreObra($preid);

	if(in_array($esdid, array(WF_TIPO_EM_REFORMULACAO,WF_TIPO_EM_CORRECAO,WF_TIPO_EM_CADASTRAMENTO))){
		if( ($retornoVerificacao !== true) && ($retornoVerificacao !== false) ){
			return $retornoVerificacao;
		} elseif( $retornoVerificacao == false ) {
			return false;
		}
	}

	$arrTipoObras = $db->pegaLinha("select pre.tooid, pto.ptoclassificacaoobra, preanoselecao, pre.ptoid
									from obras.preobra pre
										inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
									where
										pre.preid = {$preid}
									    and pre.prestatus = 'A'
									    and pto.ptostatus = 'A'");

	if($arrTipoObras['ptoclassificacaoobra'] == 'P'){
		$arrTipos = array(44,45,43,42,48,1); // Tipos "MI" e Tipos "A"
		if( !in_array($arrTipoObras['ptoid'], $arrTipos) ){
			return 'O Tipo de Obra da aba Dados do Terreno deve ser A ou Metodologia Inovadora.';
		}
	}

	$oPreObra = new PreObra();

	if( $oPreObra->verificaArquivoCorrompido( $preid ) ){
		return 'Prezada(o) Usuaria(o) - Esta obra contém arquivos corrompidos. Favor anexar novamente os documentos ou fotos anexados em vermelho clicando no botão \\\'Substituir\\\' , nas abas \\\'Cadastro de fotos do terreno\\\', \\\'Documentos Anexos\\\' e \\\'Projetos - Tipo A\\\', se houver. Posteriormente, tramitar a obra para analise do FNDE - Atenciosamente - Equipe SIMEC/PAR.';
	}

// 	$sql = "SELECT dioqtddia, diodata FROM par.diligenciaobra WHERE dioativo = 't' AND preid = ".$preid;
	$sql = "SELECT
				CAST(max(hd.htddata) as date) as diodata,
				COALESCE((SELECT dioqtddia FROM ( SELECT dioqtddia, dioid FROM par.diligenciaobra WHERE preid = $preid and dioativo = true ORDER BY dioid DESC LIMIT 1 ) as foo ), 30) as dioqtddia
			FROM workflow.historicodocumento hd
			WHERE
				docid = (SELECT docid FROM obras.preobra WHERE preid = $preid )
				AND aedid IN (2022,516,617,1815)";
	$dados = $db->pegaLinha( $sql );

	if(is_array($dados) && $dados['diodata']) {
		$arData = explode('-', $dados['diodata']);

		$ano = $arData[0];
		$mes = $arData[1];
		$dia = $arData[2];
		$dataFinal = mktime(24*$dados['dioqtddia'], 0, 0, $mes, $dia, $ano);
		$dataFormatada = date('Y-m-d',$dataFinal);

		if( $dataFormatada < date('Y-m-d') ){
			return 'Prazo de diligência expirado!';
		} else {
			return true;
		}
	} else {
		$dataFinal = calculaPrazoDiligencia( $preid );
		//$dataFinal = '2011-12-31'; // Data fixa pois o Daniel disse que fechará manualmente a análise nesse dia. Victor Benzi (24/11/11)
		$dataFinal = str_replace("-","",$dataFinal)."235959";
		if(date('YmdHis') <= $dataFinal){

			return true;

		}else{

	//		$arData_final = explode("-", $dataFinal);
			$data_formatada = substr($dataFinal, 6, 2)."/".substr($dataFinal, 4, 2)."/".substr($dataFinal, 0, 4);

			$texto = $data_formatada ? "em {$data_formatada}" : "";

			return "Prazo para retorno de diligência expirado {$texto}.";

		}
	}

//	$docid = prePegarDocid($preid);
//
//	$sql = "SELECT
//				max(hd.htddata)
//			FROM workflow.historicodocumento hd
//			WHERE
//				docid = {$docid}
//				AND aedid = 516";
//
//	$data = $db->pegaUm($sql);
//
//	$arData = explode(" ", $data);
//
//	$grupo = verificaGrupoMunicipio($preid);
//	if(!empty($arData[0])){
//		if( in_array($grupo,Array(TIPOMUN_GRUPO1,TIPOMUN_GRUPO2))){
//			$dataFinal = somar_dias_uteis($arData[0], 11);
//		}elseif( $grupo == TIPOMUN_GRUPO3 ){
//			$dataFinal = somar_dias_uteis($arData[0], 11);
//		}else{
//			return false;
//		}
//	}

}

function testaPermissao( $diretorio, $visao ) {

	if ($diretorio == 'principal/programas/proinfancia/'){

		$docid = prePegarDocid($_SESSION['par']['preid']);
		$esdid = prePegarEstadoAtual($docid);
		$perfil = pegaArrayPerfil($_SESSION['usucpf']);

		if( in_array(PAR_PERFIL_EQUIPE_MUNICIPAL,$perfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$perfil) ){
			if( $visao == 'pacAnalise' ){
				if(  !in_array( $esdid, Array(WF_TIPO_EM_CADASTRAMENTO, WF_TIPO_EM_CORRECAO) ) ){
					return false;
				}
			}
			if( $visao == 'pacAnaliseEngenheiro' ){
				if(  !in_array( $esdid, Array(WF_TIPO_EM_CORRECAO,WF_TIPO_EM_CADASTRAMENTO) ) ){
					return false;
				}
			}
		}
		if( in_array(PAR_PERFIL_ENGENHEIRO_FNDE,$perfil) ){
			if( $visao == 'pacAnalise' ){
				if(  in_array( $esdid, Array(WF_TIPO_OBRA_INDEFERIDA, WF_TIPO_OBRA_DEFERIDA, WF_TIPO_OBRA_CONDICIONADA, WF_TIPO_OBRA_INDEFERIDA_PRAZO, WF_TIPO_OBRA_APROVADA, WF_TIPO_OBRA_ARQUIVADA) ) ){
					return false;
				}
			}
		}
		if( in_array(PAR_PERFIL_COORDENADOR_GERAL,$perfil) ){
			if( $visao == 'pacAnalise' ){
				if(  in_array( $esdid, Array(WF_TIPO_OBRA_INDEFERIDA, WF_TIPO_OBRA_DEFERIDA, WF_TIPO_OBRA_CONDICIONADA, WF_TIPO_OBRA_INDEFERIDA_PRAZO, WF_TIPO_OBRA_APROVADA, WF_TIPO_OBRA_ARQUIVADA) ) ){
					return false;
				}
			}
		}
	}

	return true;
}

function pendenciaArquivoCadastramentoWF( $preid ){

	global $db;
	$oPreObra = new PreObra();

	if( !$oPreObra->verificaArquivoValidado( $preid )  && !$oPreObra->verificaArquivoCorrompido( $preid ) ){
		return 'Prezada(o) Usuaria(o) - Esta obra contém arquivos corrompidos. Favor anexar novamente os documentos ou fotos anexados em vermelho clicando no botão \\\'Substituir\\\' , nas abas \\\'Cadastro de fotos do terreno\\\', \\\'Documentos Anexos\\\' e \\\'Projetos - Tipo A\\\', se houver. Posteriormente, tramitar a obra para analise do FNDE - Atenciosamente - Equipe SIMEC/PAR.';
	}else{
		$sql = "SELECT dioqtddia, diodata FROM par.diligenciaobra WHERE dioativo = 't' AND preid = ".$preid;
		$dados = $db->pegaLinha( $sql );

		if(is_array($dados) && $dados['diodata']) {
			$arData = explode('-', $dados['diodata']);

			$ano = $arData[0];
			$mes = $arData[1];
			$dia = $arData[2];
			$dataFinal = mktime(24*$dados['dioqtddia'], 0, 0, $mes, $dia, $ano);
			$dataFormatada = date('Y-m-d',$dataFinal);

			if( $dataFormatada < date('Y-m-d') ){
				return 'Prazo de diligência expirado!';
			} else {
				return true;
			}
		} else {
			return true;
		}
	}
}

function pendenciaArquivo( $preid ){

	$oPreObra = new PreObra();

	if( !$oPreObra->verificaArquivoCorrompido( $preid ) ){
		return true;
	}
}

    function carregaAbasProFuncionario($stPaginaAtual = null){
	global $db;

	$abas = array();

	if($_SESSION['par']['prgid']){
		$sql = "
                    SELECT * FROM par.pfcurso
                    WHERE pfcstatus = 'A' AND prgid = ".$_SESSION['par']['prgid']."
                    ORDER BY pfcid
                ";
		$arCursos = $db->carregar($sql);

		$abaAdesao 		   	= "par.php?modulo=principal/programas/feirao_programas/termoadesao&acao=A";
		$abaCoordenador	   		= "par.php?modulo=principal/programas/feirao_programas/coordenador&acao=A";
		$abaTutor 		   	= "par.php?modulo=principal/programas/feirao_programas/professortutor&acao=A";
		$abaTransporte 	   		= "par.php?modulo=principal/programas/feirao_programas/transporteEscolarAcessivel&acao=A";
		$abaAguaNaEscola   		= "par.php?modulo=principal/programas/feirao_programas/aguaNaEscola&acao=A";

                $abaEJA_InformativoAdesao       = "par.php?modulo=principal/programas/feirao_programas/eja/eja_informativo_adesao&acao=A";
		$abaEJA_FormularioNovasTurmas   = "par.php?modulo=principal/programas/feirao_programas/eja/eja_quest_novas_turmas&acao=A";
		$abaEJA_ListaEscola 		= "par.php?modulo=principal/programas/feirao_programas/eja/eja_lista_escola&acao=A";

                $abaEJA_Pronatec_InfoAdesao     = "par.php?modulo=principal/programas/feirao_programas/eja_pronatec/eja_pronatec_info_adesao&acao=A";
		$abaEJA_Pronatec_Institucional  = "par.php?modulo=principal/programas/feirao_programas/eja_pronatec/eja_pronatec_institucional&acao=A";
		$abaEJA_Pronatec_Superv_demanda = "par.php?modulo=principal/programas/feirao_programas/eja_pronatec/eja_pronatec_supervisor&acao=A";
		$abaEJA_Pronatec_Questionario   = "par.php?modulo=principal/programas/feirao_programas/eja_pronatec/eja_pronatec_questionario&acao=A";
		$abaEJA_Pronatec_Documentos     = "par.php?modulo=principal/programas/feirao_programas/eja_pronatec/eja_pronatec_documentos&acao=A";

		$abaListaEscolaTerra            = "par.php?modulo=principal/programas/feirao_programas/listaEscolaTerra&acao=A";
		$abaListaEscolaEMI 		= "par.php?modulo=principal/programas/feirao_programas/listaEscolaEMI&acao=A";
		$abaMaisMedicosCP 		= "par.php?modulo=principal/programas/feirao_programas/maisMedicosCondicoesPart&acao=A";
		$abaMaisMedicosIM 		= "par.php?modulo=principal/programas/feirao_programas/maisMedicosInforMunicipio&acao=A";
		$abaMaisMedicosDados            = "par.php?modulo=principal/programas/feirao_programas/maisMedicosDadosRepres&acao=A";
		$abaMaisMedicosAcoes            = "par.php?modulo=principal/programas/feirao_programas/maisMedicosAcoesMunicipio&acao=A";
		$abaMaisMedicosAdesao           = "par.php?modulo=principal/programas/feirao_programas/maisMedicosAdesao&acao=A";
		$abaMaisMedicosDoc		= "par.php?modulo=principal/programas/feirao_programas/maisMedicosDocumentos&acao=A";
		$abaMaisMedicosRM		= "par.php?modulo=principal/programas/feirao_programas/maisMedicosRecursoMunicipio&acao=A";
		$abaMaisMedicosA		= "par.php?modulo=principal/programas/feirao_programas/maisMedicosAnalise&acao=A";
		$abaMaisMedicosQuestAval        = "par.php?modulo=principal/programas/feirao_programas/maisMedicoQuestAvaliacao&acao=A";
		$abaMaisMedicosDocAval          = "par.php?modulo=principal/programas/feirao_programas/maisMedicoDocAvaliacao&acao=A";
		$abaMaisMedicosRecursoQuest     = "par.php?modulo=principal/programas/feirao_programas/maisMedicosRecursoQuest&acao=A";
                $abaMaisMedicosRecursoNotaTecnica     = "par.php?modulo=principal/programas/feirao_programas/maisMedicosManifestacaoSobreVisitaInLoco&acao=A";

                $abaMaisMedicos_2015_participacao = "par.php?modulo=principal/programas/feirao_programas/mais_medicos_2015/condicoes_participacao&acao=A";
                $abaMaisMedicos_2015_info_municip = "par.php?modulo=principal/programas/feirao_programas/mais_medicos_2015/informacao_municipio&acao=A";
                $abaMaisMedicos_2015_dados_repres = "par.php?modulo=principal/programas/feirao_programas/mais_medicos_2015/dados_representantes&acao=A";
                $abaMaisMedicos_2015_adesao_progr = "par.php?modulo=principal/programas/feirao_programas/mais_medicos_2015/adesao_programa&acao=A";
                $abaMaisMedicos_2015_documentos   = "par.php?modulo=principal/programas/feirao_programas/mais_medicos_2015/documentos_ad&acao=A";

		array_push($abas, array("descricao" => "Adesão", "link" => $abaAdesao));

		if($_SESSION['par']['itrid'] != 2 && $_SESSION['par']['prgid'] == 60){
			array_push($abas, array("descricao" => "Coordenador do Profuncionário", "link" => $abaCoordenador));
		}

		if($_SESSION['par']['prgid'] == 60){
			array_push($abas, array("descricao" => "Professor Tutor", "link" => $abaTutor));
		}

		//if($_SESSION['par']['pfaid'] == 5){
		if($_SESSION['par']['pfaid'] == 11){
			array_push($abas, array("descricao" => "Transporte Escolar Acessível", "link" => $abaTransporte));
		}

		if($_SESSION['par']['prgid'] == 183){
			array_push($abas, array("descricao" => "Fotos - Água na Escola", "link" => $abaAguaNaEscola));
		}

		if($_SESSION['par']['prgid'] == PROG_PAR_PROGRAMA_FINANCIAMENTO_EJA){

                    array_shift($abas);

                    $aderiu = verificaAdesaoEJA();
                    if( $aderiu != 'S' ){
                        array_push($abas, array("descricao" => "Informativo para Adesão", "link" => $abaEJA_InformativoAdesao));

                        if( $_SESSION['continuaAdesao'] == 'S'){
                            array_push($abas, array("descricao" => "Adesão", "link" => $abaAdesao));
                        }
                    }else{
                        array_push($abas, array("descricao" => "Informativo para Adesão", "link" => $abaEJA_InformativoAdesao));
                        array_push($abas, array("descricao" => "Adesão", "link" => $abaAdesao));
                        array_push($abas, array("descricao" => "Previsão de Ofertas", "link" => $abaEJA_FormularioNovasTurmas));
                        array_push($abas, array("descricao" => "Lista de Escolas", "link" => $abaEJA_ListaEscola));
                    }
		}

		if($_SESSION['par']['prgid'] == PROG_PAR_EJA_PRONATEC){

                    array_shift($abas);

                    $aderiu = verificaAdesaoEJA_PRONATEC();
                    if( $aderiu != 'S' ){
                        array_push($abas, array("descricao" => "Informativo para Adesão", "link" => $abaEJA_Pronatec_InfoAdesao));

                        if( $_SESSION['continuaAdesaoPronatec'] == 'S'){
                            array_push($abas, array("descricao" => "Adesão", "link" => $abaAdesao));
                        }
                    }else{
                        array_push($abas, array("descricao" => "Informativo para Adesão", "link" => $abaEJA_Pronatec_InfoAdesao));
                        array_push($abas, array("descricao" => "Adesão", "link" => $abaAdesao));
                        array_push($abas, array("descricao" => "Cadastro Institucional", "link" => $abaEJA_Pronatec_Institucional));
                        array_push($abas, array("descricao" => "Supervisor de Demandas", "link" => $abaEJA_Pronatec_Superv_demanda));
                        array_push($abas, array("descricao" => "Estimativa de Matrículas", "link" => $abaEJA_Pronatec_Questionario));
                        array_push($abas, array("descricao" => "Anexar Documentos", "link" => $abaEJA_Pronatec_Documentos));
                    }
		}

                #MAIS MÉDICO - NOVO EDITAL 2015
		if($_SESSION['par']['prgid'] == PROG_PAR_MAIS_MEDICO_NOVO_2015){
                    array_shift($abas);

                    $aderiu = verificaAdesaoMaisMedicoNovo();
                    if(  $_SESSION['par']['rqmid'] != '' ){
                        $resp = verificaInformacaoMunicipio( $_SESSION['par']['rqmid'] );
                    }

                    if( $aderiu != 'S' ){
                        if( $aderiu == 'N' ){
                            array_push($abas, array("descricao" => "Condições de Participação", "link" => $abaMaisMedicos_2015_participacao));
                        }else{

                            array_push($abas, array("descricao" => "Condições de Participação", "link" => $abaMaisMedicos_2015_participacao));

                            if( $resp['rqmaceitetermoresidencia'] == 't'){
                                array_push($abas, array("descricao" => "Informações do Município", "link" => $abaMaisMedicos_2015_info_municip));
                                array_push($abas, array("descricao" => "Dados dos Representantes", "link" => $abaMaisMedicos_2015_dados_repres));
                                array_push($abas, array("descricao" => "Adesão ao Programa", "link" => $abaMaisMedicos_2015_adesao_progr));
                            }else{
                                if( $_SESSION['continuaAdesaoMaisMedico_2015'] == 'S'){
                                    array_push($abas, array("descricao" => "Informações do Município", "link" => $abaMaisMedicos_2015_info_municip));
                                }
                            }
                        }
                    }else{
                        array_push($abas, array("descricao" => "Condições de Participação", "link" => $abaMaisMedicos_2015_participacao));
                        array_push($abas, array("descricao" => "Informações do Município", "link" => $abaMaisMedicos_2015_info_municip));
                        array_push($abas, array("descricao" => "Dados dos Representantes", "link" => $abaMaisMedicos_2015_dados_repres));
                        array_push($abas, array("descricao" => "Adesão ao Programa", "link" => $abaMaisMedicos_2015_adesao_progr));
                        array_push($abas, array("descricao" => "Documentos Município", "link" => $abaMaisMedicos_2015_documentos));

                        if( $_SESSION['par']['muncod'] != 5201405 && $_SESSION['par']['muncod'] != 3143906
                        //                                            && $_SESSION['par']['muncod'] != 2208007
                        ){
                        	array_push($abas, array("descricao" => "Avaliação in loco", "link" => $abaMaisMedicosQuestAval));
                        }

                    }
		}

		if($_SESSION['par']['prgid'] == 224){
			array_push($abas, array("descricao" => "Lista de Escolas", "link" => $abaListaEscolaTerra));
		}

		if($_SESSION['par']['prgid'] == 220){
			array_push($abas, array("descricao" => "Lista de Escolas", "link" => $abaListaEscolaEMI));
		}

		if($_SESSION['par']['prgid'] == 228){
			array_shift($abas);
			array_push($abas, array("descricao" => "Condições de Participação", "link" => $abaMaisMedicosCP));

			if( $_SESSION['abaMaisMedicosIM'] == 'S'){
                                array_push($abas, array("descricao" => "Informações do Município", "link" => $abaMaisMedicosIM));

				if($_SESSION['abaMaisMedicos'] == 'S'){
					array_push($abas, array("descricao" => "Dados Representantes", "link" => $abaMaisMedicosDados));
					array_push($abas, array("descricao" => "Projetos de Melhoria", "link" => $abaMaisMedicosAcoes));
					array_push($abas, array("descricao" => "Adesão", "link" => $abaMaisMedicosAdesao));
					array_push($abas, array("descricao" => "Documentos", "link" => $abaMaisMedicosDoc));
					array_push($abas, array("descricao" => "Análise", "link" => $abaMaisMedicosA));
					array_push($abas, array("descricao" => "Recurso Município", "link" => $abaMaisMedicosRM));


                                    	// regra para tirar a respectiva aba para o caso dos 3 municipios mencionados.
                                    	if( $_SESSION['par']['muncod'] != 5201405 && $_SESSION['par']['muncod'] != 3143906
//                                            && $_SESSION['par']['muncod'] != 2208007
                                        ){
                                        	array_push($abas, array("descricao" => "Avaliação in loco", "link" => $abaMaisMedicosQuestAval));
                                        }


                                        // if( $_SESSION['baselogin'] == 'simec_espelho_producao' || $_SESSION['baselogin'] == 'simec_desenvolvimento' ){

                                            array_push($abas, array("descricao" => "Documentos Avaliação", "link" => $abaMaisMedicosDocAval));
                                            //array_push($abas, array("descricao" => "Recurso Questionário Avaliação", "link" => $abaMaisMedicosRecursoQuest));
                                        // }
				}
			}

			if( $_SESSION['abaMaisMedicosManifestacao'] == 'S' ){
				array_push($abas, array("descricao" => "Manifestação Visita in loco", "link" => $abaMaisMedicosRecursoNotaTecnica));
			}
		}

		if($arCursos){
                    foreach($arCursos as $curso){
                        if($_SESSION['par']['prgid'] == PROG_PAR_ADESAO_CONSELHO_ESCOLAR){
                            array_push($abas, array("descricao" => $curso['pfcdescricao'], "link" => 'par.php?modulo=principal/programas/feirao_programas/conselho-escolar&acao=A&pfcid='.$curso['pfcid']));
                        } else {
                            array_push($abas, array("descricao" => $curso['pfcdescricao'], "link" => 'par.php?modulo=principal/programas/feirao_programas/pfcursos&acao=A&pfcid='.$curso['pfcid']));
                        }
                    }
		}
		return montarAbasArray($abas, $stPaginaAtual, $win);
	}
	else{
		return '';
	}
    }

function verificaAdesaoEJA(){
    global $db;

    if( $_SESSION['par']['adpid'] ){
        $sql = "
            SELECT  max(adpid),
                    adpresposta
            FROM par.pfadesaoprograma
            WHERE adpid = ".$_SESSION['par']['adpid']."

            GROUP BY adpid, adpresposta
            ORDER BY adpid DESC
            LIMIT 1
        ";
        $adpresposta = $db->pegaLinha($sql);
        return $adpresposta['adpresposta'];

    }else{
        return '';
    }
}

function verificaAdesaoEJA_PRONATEC(){
    global $db;

    if( $_SESSION['par']['adpid'] ){
        $sql = "
            SELECT  max(adpid),
                    adpresposta
            FROM par.pfadesaoprograma
            WHERE adpid = ".$_SESSION['par']['adpid']."

            GROUP BY adpid, adpresposta
            ORDER BY adpid DESC
            LIMIT 1
        ";
        $adpresposta = $db->pegaLinha($sql);
        return $adpresposta['adpresposta'];

    }else{
        return '';
    }
}

function verificaAdesaoMaisMedicoNovo(){
    global $db;

    if( $_SESSION['par']['adpid'] ){
        $sql = "
            SELECT  max(adpid),
                    adpresposta
            FROM par.pfadesaoprograma
            WHERE adpid = ".$_SESSION['par']['adpid']."

            GROUP BY adpid, adpresposta
            ORDER BY adpid DESC
            LIMIT 1
        ";
        $adpresposta = $db->pegaLinha($sql);
        return $adpresposta['adpresposta'];

    }else{
        return '';
    }
}

function verificaInformacaoMunicipio( $rqmid ){
    global $db;

    $sql = "
        SELECT * FROM par.respquestaomaismedico WHERE rqmid = {$rqmid}
    ";
    return $db->pegaLinha($sql);
}

function recuperaMunicipioEstado()
{
	global $db;

	if($_SESSION['par']['muncod']){
		$stCampo = "mun.mundescricao || ' - ' || est.estuf as descricao";
		$stInner = "LEFT JOIN territorios.municipio mun ON mun.estuf = est.estuf";
		$stWhere = "WHERE mun.muncod = '{$_SESSION['par']['muncod']}'";
	}else{
		$stCampo = "est.estdescricao || ' - ' || est.estuf as descricao";
		$stWhere = "WHERE est.estuf = '{$_SESSION['par']['estuf']}'";
	}

	$sql = "SELECT
				{$stCampo}
			FROM territorios.estado est
			{$stInner}
			{$stWhere}
			LIMIT 1";

	return $db->pegaUm($sql);
}

function atualizaValorObra( $preid, $testemi = null ){

	global $db;

	if( $preid ){
		$where = "WHERE preid = {$preid}";
	}
	$oSubacaoControle = new SubacaoControle();
	$tipoObra = $oSubacaoControle->verificaTipoObra($preid, SIS_OBRAS);
	//$tipoFundacao = $oSubacaoControle->verificaTipoFundacao($preid);

	if( $testemi == 'mi' ){
		$sql = "UPDATE obras.preobra po
				SET
					prevalorobra = (select
										sum(ic.itcvalorunitario * itcquantidade) as valor
									from obras.preitenscomposicaomi ic
									inner join obras.preobra pre on pre.preid = ic.preid and pre.ptoid = ic.ptoid
									where
										pre.prestatus = 'A'
										and ic.preid = po.preid
										and ic.itcstatus = 'A'
										and pre.ptoid = $tipoObra
	                                    and ic.itccodigoitemcodigo <> ''
	                                    --and ic.itctipofundacao = '$tipoFundacao'
	                                    )
				{$where}";
	} else {
		$sql = "UPDATE obras.preobra po
				SET
					prevalorobra = (select
										sum(plo.ppovalorunitario * coalesce( pirqtd, itcquantidade) ) as valor
									from obras.preplanilhaorcamentaria plo
									inner join obras.preitenscomposicao 		ic on ic.itcid = plo.itcid
									inner join obras.preobra 					pre on pre.preid = plo.preid AND pre.ptoid = ic.ptoid
									LEFT  JOIN obras.preitencomposicao_regiao 	pir ON pir.itcid = ic.itcid AND pir.estuf = pre.estuf
									where
										pre.prestatus = 'A'
										and plo.preid = po.preid
										and ic.itcstatus = 'A'
										and pre.ptoid = $tipoObra
	                                    and ic.itccodigoitemcodigo <> ''
	                                    --and ic.itctipofundacao = '$tipoFundacao'
	                                    )
				{$where}";
	}
	$db->executar($sql);
	$db->commit();
}

/*
* Função para geração de pdf por meio do ws-mec
* retorna string de bits para escrita do arquivo
*/
function pdf( $string_html , $output = false, $id = '', $titulo = 'pdf_' ) {

	$content = http_build_query( array('conteudoHtml'=> $string_html ) );
	$context = stream_context_create(array( 'http'    => array(
							                'method'  => 'POST',
							                'content' => $content ) ));

	$contents = file_get_contents('http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context );

	if( $output ){
		$id = $id != '' ? $id : uniqid();
		header('Content-Type: application/pdf');
		header("Content-Disposition: attachment; filename={$titulo}{$id}.pdf");
		echo $contents;
		exit();
	}
	return $contents;

}


/*
 * Função de aceite do termo de compromisso do programa PROFuncionario
 */
function respondeTermoProFuncionario($dados) {

    global $db;

    extract($dados);

    if ($resposta || $pacto) {

        $adpid = $db->pegaUm("select adpid from par.pfadesaoprograma where pfaid = " . $_SESSION['par']['pfaid'] . " and inuid = " . $_SESSION['par']['inuid']);

        $pfaano = $db->pegaUm("select pfaano from par.pfadesao where pfaid = " . $_SESSION['par']['pfaid']);
        if(!$pfaano) $pfaano = date("Y");
        $dados['pfaano'] = $pfaano;

        if ($pacto == 'true') {
            $pacto = $resposta;
            $resposta = '';
        }

        if ($resposta == 'N') {
            $adpjustificativaresposta = "'" . $adpjustificativaresposta . "'";
        } elseif ($resposta == 'S') {
            $adpjustificativaresposta = 'null';
        }

        if (!$adpid) {

            $tapid = $tapid ? $tapid : 'null';

            $sql = "
                INSERT INTO par.pfadesaoprograma(
                        pfaid,
                        adpano,
                        inuid,

                        " . ( $resposta ? 'adpdataresposta,' : '' ) . "
                        " . ( $resposta ? 'adpresposta,' : '' ) . "
                        " . ( $pacto ? 'adprespostapacto,' : '' ) . "
                        " . ( $pacto ? 'adpdatarespostapacto,' : '' ) . "
                        " . ( $resposta ? 'adpjustificativaresposta,' : '' ) . "

                        usucpf,
                        tapid
                    )VALUES(
                        " . $_SESSION['par']['pfaid'] . ",
                        '" . $pfaano . "',
                        " . $_SESSION['par']['inuid'] . ",

                        " . ( $resposta ? "'" . date("Y-m-d G:i:s") . "'," : '' ) . "
                        " . ( $resposta ? "'" . $resposta . "'," : '' ) . "
                        " . ( $pacto ? "'" . $pacto . "'," : '' ) . "
                        " . ( $pacto ? "'" . date("Y-m-d G:i:s") . "'," : '' ) . "
                        " . ( $resposta ? $adpjustificativaresposta . "," : '' ) . "

                        '" . $_SESSION['usucpf'] . "',
                    " . $tapid . "
                    ) returning adpid ";

            $adpid = $db->pegaUm($sql);
        } else {

            $sql = "UPDATE par.pfadesaoprograma
                        SET " . ( $resposta ? "adpdataresposta=now()," : '' ) . "
                            " . ( $resposta ? "adpresposta='" . $resposta . "'," : '' ) . "
                            " . ( $pacto ? "adprespostapacto = '" . $pacto . "'," : '' ) . "
                            " . ( $pacto ? "adpdatarespostapacto = now()," : '' ) . "
                            " . ( $resposta ? "adpjustificativaresposta = " . $adpjustificativaresposta . "," : '' ) . "
                            usucpf='" . $_SESSION['usucpf'] . "'
                     WHERE adpid = " . $adpid;
            $db->executar($sql);
        }
        $db->commit();

        inserirProgramaHistorico($dados, $adpid);

        // Recupera configurações do programa
        if ($_SESSION['par']['estuf'] == 'DF' && $_SESSION['par']['itrid'] == 1) {
            $tprcod = 2;
        } else {
            $tprcod = $_SESSION['par']['muncod'] == '' ? 1 : 2;
        }

        $sql = "SELECT
                    tap.tapid,
                    tap.taptermo,
                    tap.tapinstrucao,
                    tap.tapmsg,
                    tap.tappacto,
                    tap.tapmsgaceitetermo,
                tap.tapmsgaceitepacto,
                tap.tapmsgnaoaceitetermo,
                tap.tapmsgnaoaceitepacto,
                tapredireciona
            FROM
                    par.pftermoadesaoprograma tap
            WHERE
                    tap.prgid = " . $_SESSION['par']['prgid'] . "
            AND
                    tap.tapano = " . $_SESSION['par']['pfaano'] . "
            AND
                    tap.tapstatus = 'A'
            AND
                    tap.tprcod = " . $tprcod;

        $dados = $db->pegaLinha($sql);

        echo "<style> .ui-dialog-buttonset, .dialog-content{ font-size:12px; } .ui-dialog-titlebar{ font-size:14px; } .ui-dialog-titlebar-close{ display: none; }</style>";
        echo "<script>jQuery.noConflict();</script>";
        echo '
            <div id="dialog-confirm" title="Confirmação" style="display:none;">
                <p>
                    <span class="ui-icon ui-icon-alert" style="float:left; margin:0 7px 5px 0;"></span>
                    <div id="dialog-content" style="font-size:12px;"></div>
                </p>
            </div>';

        if ($pacto == 'S') {
            $msg = '';
            if ( !empty($dados['tapmsgaceitepacto']) )
                $msg = $dados['tapmsgaceitepacto'];

            $stAlerta = '';

            if ($msg) {
                $stAlerta = "
                    jQuery(function(){
                        jQuery( '#dialog-confirm' ).attr({'title':'Alerta'});
                        jQuery( '#dialog:ui-dialog' ).dialog( 'destroy' );
                        jQuery( '#dialog-content' ).html('{$msg}');
                        jQuery( '#dialog-confirm' ).dialog({
                            resizable: false,
                            width: 600,
                            modal: true,
                            buttons: {
                                'Ok': function() {
                                    jQuery( this ).dialog( 'close' );
                                    window.location.href='par.php?modulo=principal/programas/feirao_programas/termoadesao&acao=A&pfaid={$_SESSION['par']['pfaid']}';
                                }
                            }
                        });
                    });
                ";
            }
            echo '<script type="text/javascript">' . $stAlerta . '</script>';

        } else if ($resposta == 'S') {

            // Verifica mensagem de alerta
            $msg = 'Operação realizada com sucesso.';
            if (!empty($dados['tapmsgaceitetermo']))
                $msg = $dados['tapmsgaceitetermo'];

            // Verifica redirecionemanto de aba
            if($_SESSION['par']['prgid'] != PROG_PAR_PROGRAMA_FINANCIAMENTO_EJA){
                $stUrl = 'par.php?modulo=principal/programas/feirao_programas/termoadesao&acao=A&pfaid=' . $_SESSION['par']['pfaid'] . '';
            }else{
                $stUrl = "par.php?modulo=principal/programas/feirao_programas/eja/eja_quest_novas_turmas&acao=A&pfaid={$_SESSION['par']['pfaid']}";
            }

            if (!empty($dados['tapredireciona'])) {
                $stUrl = $dados['tapredireciona'];
            }

            $stAlerta = '';
            if ($msg) {
                $stAlerta = "
                    jQuery(function(){
                        jQuery( '#dialog-confirm' ).attr({'title':'Alerta'});
                        jQuery( '#dialog:ui-dialog' ).dialog( 'destroy' );
                        jQuery( '#dialog-content' ).html('{$msg}');
                        jQuery( '#dialog-confirm' ).dialog({
                            resizable: false,
                            width: 600,
                            modal: true,
                            buttons: {
                                'Ok': function() {
                                    jQuery( this ).dialog( 'close' );
                                    window.location.href='{$stUrl}';
                                }
                            }
                        });
                    });";
            }

            echo '<script type="text/javascript">' . $stAlerta . '</script>';
        } else {

            $msg = 'Operação realizada com sucesso.';
            if (!empty($dados['tapmsgnaoaceitetermo']) && $resposta == 'N')
                $msg = $dados['tapmsgnaoaceitetermo'];

            if (!empty($dados['tapmsgnaoaceitepacto']) && $pacto == 'N')
                $msg = $dados['tapmsgnaoaceitepacto'];

            $stAcaoCancelar = "jQuery( this ).dialog( 'close' );
							   document.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';";

            if ($_SESSION['par']['prgid'] == 157 && $resposta == 'N') {

                $stAcaoCancelar = "
									jQuery( '#dialog:ui-dialog' ).dialog( 'destroy' );
									jQuery( '#dialog-content' ).html('Sua Secretaria de Educação concluiu a adesão ao Pacto Nacional pela Alfabetização na Idade Certa. Em breve, o Ministério da Educação entrará em contato. Para saber mais, acesse o portal do MEC: <a href=\"http://www.mec.gov.br\" target=\"_blank\">http://www.mec.gov.br</a>.');
									jQuery( '#dialog-confirm' ).dialog({
										resizable: false,
										width: 600,
										modal: true,
										buttons: {
											'Ok': function() {

												jQuery.ajax({
														url		: 'par.php?modulo=principal/programas/feirao_programas/termoadesao&acao=A&pfaid={$_REQUEST['pfaid']}',
														type	: 'post',
														data	: 'enviaEmail=true',
														success	: function(e){
															jQuery( this ).dialog( 'close' );
															document.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';
														}
												});
											}
										}
									});
									";
            }

            $stUrl = "par.php?modulo=principal/programas/feirao_programas/termoadesao&acao=A&pfaid={$_SESSION['par']['pfaid']}&alerta=false";
            $stAlerta = "jQuery(function(){

							jQuery( '#dialog:ui-dialog' ).dialog( 'destroy' );
							jQuery( '#dialog-content' ).html('{$msg}');
							jQuery( '#dialog-confirm' ).dialog({
								resizable: false,
								width: 600,
								//height:210,
								modal: true,
								buttons: {
									'Sim': function() {
										jQuery( this ).dialog( 'close' );
										window.location.href = '{$stUrl}';
									},
									'Não': function() {
										{$stAcaoCancelar}
									}
								}
							});
						});";

            echo '<script type="text/javascript">
					' . $stAlerta . '
		    	  </script>';

            die;
        }
    }
}

function inserirProgramaHistorico($dados, $adpid){
	global $db;

	extract($dados);


	$sql = "INSERT INTO par.pfadesaoprogramahistorico(
            adpid, aphano, inuid, aphdataresposta,
            aphresposta, usucpf, tapid, pfaid)
		    VALUES ($adpid,
		    		'".$pfaano."',
		    		".$_SESSION['par']['inuid'].",
		    		'".date("Y-m-d G:i:s")."',
					'".$resposta."',
					'".$_SESSION['usucpf']."',
				    ".$tapid.",
		            ".$_SESSION['par']['pfaid'].")";
	$db->executar($sql);
	$db->commit();

	return true;
}


/*
 * Função de cancelamento do termo de compromisso do programa PROFuncionario
 * */
function cancelaTermoProFuncionario($dados){

	global $db;

	if($dados['pacto'] == 'true'){
		$stUpdate = "adprespostapacto = 'C',";
	}else{
		$stUpdate = "adpresposta = 'C',";
	}

	extract($dados);

        $adpid = $db->pegaUm( "select adpid from par.pfadesaoprograma where pfaid = {$_SESSION['par']['pfaid']} and inuid = {$_SESSION['par']['inuid']}" );

	$sql = "
            UPDATE par.pfadesaoprograma
                SET adpdataresposta = now(),
                    {$stUpdate}
                    usucpf = '{$_SESSION['usucpf']}'
            WHERE adpid = {$adpid}
        ";
	$db->executar($sql);

	inserirProgramaHistorico($dados, $adpid);

	/*
	$sql = "DELETE FROM par.pfadesaoprograma
			WHERE inuid = ".$_SESSION['par']['inuid']."
			AND pfaid = ".$_SESSION['par']['pfaid'];

	$db->executar($sql);
	*/
	$db->commit();
	$db->sucesso('principal/planoTrabalho&acao=A&tipoDiagnostico=programa');
}


function pgVerificaEstado( $esdid ){

	global $db;

	$sql = "SELECT
                        esdid
                FROM
                        workflow.estadodocumento
                WHERE
                        esdid = {$esdid}";

	return $db->pegaUm( $sql );

}

function pgMaisMedicosCriarDocumento($rqmid) {
    global $db;

    if (empty($rqmid)) {
        return false;
    }

    $docid = pgPegarDocidMaisMedicos( $rqmid );
    if (!$docid) {
        if( $_SESSION['par']['prgid'] == PROG_PAR_MAIS_MEDICO_NOVO_2015 ){
            $docdsc = "Mais Médicos Novo edital 2015";
            $docid = wf_cadastrarDocumento(WF_TPDID_MAIS_MEDICOS_NOVO_2015, $docdsc);
        }else{
            $docdsc = "Mais Médicos";
            $docid = wf_cadastrarDocumento(WF_TPDID_MAIS_MEDICOS, $docdsc);
        }
        if ($rqmid) {
            $sql = "
                UPDATE par.respquestaomaismedico
                    SET docid = {$docid}
                WHERE rqmid = {$rqmid}
            ";
            $db->executar($sql);
            $db->commit();
            return $docid;
        } else {
            return false;
        }
    } else {
        return $docid;
    }
}

function pgPegarDocidMaisMedicos($rqmid) {
    global $db;

    $sql = " SELECT docid FROM par.respquestaomaismedico WHERE rqmid = {$rqmid} ";
    return (integer) $db->pegaUm($sql);
}

function pgCriarDocumento($adpid) {

    global $db;

    require_once APPRAIZ . 'includes/workflow.php';

    $docid = pgPegarDocid($adpid);

    if (!$docid) {

        if ($_SESSION['par']['estuf'] == 'DF' && $_SESSION['par']['itrid'] == 1) {
            $tprcod = 2;
        } else {
            $tprcod = $_SESSION['par']['muncod'] == '' ? 1 : 2;
        }

        // Recupera fluxo programa
        $sql = "SELECT
                        tpdid
                FROM
                        par.pftermoadesaoprograma tap
                WHERE
                        tap.prgid = " . $_SESSION['par']['prgid'] . "
                AND
                        tap.tapano = " . $_SESSION['par']['pfaano'] . "
                AND
                        tap.tapstatus = 'A'
                AND
                        tap.tprcod = " . $tprcod;

        $tpdid = $db->pegaUm($sql);

        // recupera o tipo do documento
        $tpdid = $tpdid ? $tpdid : WF_FLUXO_PROGRAMAS_FORMACAO;

        // descrição do documento
        $docdsc = "Fluxo de programas de formação - ID " . $adpid;

        // cria documento do WORKFLOW
        $docid = wf_cadastrarDocumento($tpdid, $docdsc);

        // atualiza pap do EMI
        $sql = "UPDATE
                        par.pfadesaoprograma
                SET
                        docid = {$docid}
                WHERE
                        adpid = {$adpid}";

        $db->executar($sql);
        $db->commit();
    }

    return $docid;
}

function pgTransporteCriarDocumento( $adpid ) {

	global $db;

	require_once APPRAIZ . 'includes/workflow.php';

	$docid = pgPegarDocid( $adpid );

	if( !$docid ) {

		// recupera o tipo do documento
		$tpdid = WF_FLUXO_PRO_TRANSPORTE;

		// descrição do documento
		$docdsc = "Fluxo de programas de Transporte Escolar Acessível - ID " . $adpid;

		// cria documento do WORKFLOW
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );

		// atualiza pap do EMI
		$sql = "UPDATE
					par.pfadesaoprograma
				SET
					docid = {$docid}
				WHERE
					adpid = {$adpid}";

		$db->executar( $sql );
		$db->commit();
	}

	return $docid;

}

function pgAguaEscolaCriarDocumento( $adpid ) {

	global $db;

	require_once APPRAIZ . 'includes/workflow.php';

	$docid = pgPegarDocid( $adpid );

	if( !$docid ) {

		// recupera o tipo do documento
		$tpdid = WF_FLUXO_PRO_AGUA_ESCOLA;

		// descrição do documento
		$docdsc = "Fluxo de programa de Água na Escola - ID " . $adpid;

		// cria documento do WORKFLOW
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );

		// atualiza pap do EMI
		$sql = "UPDATE
					par.pfadesaoprograma
				SET
					docid = {$docid}
				WHERE
					adpid = {$adpid}";

		$db->executar( $sql );
		$db->commit();
	}

	return $docid;

}

function pgPegarDocid( $adpid ) {

	global $db;

	$sql = "SELECT
				docid
			FROM
				par.pfadesaoprograma
			WHERE
			 	adpid = " . (integer) $adpid;

	return (integer) $db->pegaUm( $sql );

}

function pgPegarEstadoAtual( $docid ) {

	global $db;

	$sql = "SELECT
				ed.esdid
			FROM
				workflow.documento d
			INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
			WHERE
				d.docid = " . $docid;

	$estado = (integer) $db->pegaUm( $sql );

	return $estado;

}




function exibeMapaRegionalizador($cxpid = null){

	?>
	<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" class="tabela">
		<tr>
			<td valign="top" align="center" width="450">
				<fieldset style="background: #ffffff; width: 450px;"  >
					<legend> Selecione o Estado para emitir o relatório </legend>

					<div style="width: 100%;" id="containerMapa" >
						<img src="/imagens/mapa_brasil.png" width="444" height="357" border="0" usemap="#mapaBrasil" />
						<map name="mapaBrasil" id="mapaBrasil">
							<area shape="rect" coords="388,15,427,48"   style="cursor:pointer;" onclick="exibeRegionalizador('todas');" title="Brasil"/>
							<area shape="rect" coords="48,124,74,151"   style="cursor:pointer;" onclick="exibeRegionalizador('AC');" title="Acre"/>
							<area shape="rect" coords="364,147,432,161" style="cursor:pointer;" onclick="exibeRegionalizador('AL');" title="Alagoas"/>
							<area shape="rect" coords="202,27,233,56"   style="cursor:pointer;" onclick="exibeRegionalizador('AP');" title="Amapá"/>
							<area shape="rect" coords="89,76,133,107"   style="cursor:pointer;" onclick="exibeRegionalizador('AM');" title="Amazonas"/>
							<area shape="rect" coords="294,155,320,183" style="cursor:pointer;" onclick="exibeRegionalizador('BA');" title="Bahia"/>
							<area shape="rect" coords="311,86,341,114"  style="cursor:pointer;" onclick="exibeRegionalizador('CE');" title="Ceará"/>
							<area shape="rect" coords="244,171,281,197" style="cursor:pointer;" onclick="exibeRegionalizador('DF');" title="Distrito Federal"/>
							<area shape="rect" coords="331,215,369,242" style="cursor:pointer;" onclick="exibeRegionalizador('ES');" title="Espírito Santo"/>
							<area shape="rect" coords="217,187,243,218" style="cursor:pointer;" onclick="exibeRegionalizador('GO');" title="Goiás"/>
							<area shape="rect" coords="154,155,210,186" style="cursor:pointer;" onclick="exibeRegionalizador('MT');" title="Mato Grosso"/>
							<area shape="rect" coords="156,219,202,246" style="cursor:pointer;" onclick="exibeRegionalizador('MS');" title="Mato Grosso do Sul"/>
							<area shape="rect" coords="248,80,301,111"  style="cursor:pointer;" onclick="exibeRegionalizador('MA');" title="Maranhão"/>
							<area shape="rect" coords="264,206,295,235" style="cursor:pointer;" onclick="exibeRegionalizador('MG');" title="Minas Gerais"/>
							<area shape="rect" coords="188,84,217,112"  style="cursor:pointer;" onclick="exibeRegionalizador('PA');" title="Pará"/>
							<area shape="rect" coords="368,112,433,130" style="cursor:pointer;" onclick="exibeRegionalizador('PB');" title="Paraíba"/>
							<area shape="rect" coords="201,262,231,289" style="cursor:pointer;" onclick="exibeRegionalizador('PR');" title="Paraná"/>
							<area shape="rect" coords="369,131,454,147" style="cursor:pointer;" onclick="exibeRegionalizador('PE');" title="Pernambuco"/>
							<area shape="rect" coords="285,116,313,146" style="cursor:pointer;" onclick="exibeRegionalizador('PI');" title="Piauí"/>
							<area shape="rect" coords="349,83,383,108"  style="cursor:pointer;" onclick="exibeRegionalizador('RN');" title="Rio Grande do Norte"/>
							<area shape="rect" coords="189,310,224,337" style="cursor:pointer;" onclick="exibeRegionalizador('RS');" title="Rio Grande do Sul"/>
							<area shape="rect" coords="302,250,334,281" style="cursor:pointer;" onclick="exibeRegionalizador('RJ');" title="Rio de Janeiro"/>
							<area shape="rect" coords="98,139,141,169"  style="cursor:pointer;" onclick="exibeRegionalizador('RO');" title="Rondônia"/>
							<area shape="rect" coords="112,24,147,56"   style="cursor:pointer;" onclick="exibeRegionalizador('RR');" title="Roraima"/>
							<area shape="rect" coords="228,293,272,313" style="cursor:pointer;" onclick="exibeRegionalizador('SC');" title="Santa Catarina"/>
							<area shape="rect" coords="233,243,268,270" style="cursor:pointer;" onclick="exibeRegionalizador('SP');" title="São Paulo"/>
							<area shape="rect" coords="337,161,401,178" style="cursor:pointer;" onclick="exibeRegionalizador('SE');" title="Sergipe"/>
							<area shape="rect" coords="227,130,270,163" style="cursor:pointer;" onclick="exibeRegionalizador('TO');" title="Tocantins"/>
							<!--<area shape="rect" coords="17,264,85,282"   style="cursor:pointer;" onclick="exibeRegionalizador('Norte','<?=$arrDados['cxpid']?>');" title="Norte" />
							<area shape="rect" coords="16,281,94,296"   style="cursor:pointer;" onclick="exibeRegionalizador('Nordeste','<?=$arrDados['cxpid']?>');" title="Nordeste" />
							<area shape="rect" coords="15,296,112,312"  style="cursor:pointer;" onclick="exibeRegionalizador('Centro-Oeste','<?=$arrDados['cxpid']?>');" title="Centro-Oeste" />
							<area shape="rect" coords="14,312,100,329"  style="cursor:pointer;" onclick="exibeRegionalizador('Sudeste','<?=$arrDados['cxpid']?>');" title="Sudeste" />
							<area shape="rect" coords="13,329,68,344"   style="cursor:pointer;" onclick="exibeRegionalizador('Sul','<?=$arrDados['cxpid']?>')" title="Sul" />
						--></map>
					</div>
				</fieldset>
			</td>
		</tr>
	</table>
	<?php
}

function salvarPregao()
{
global $db;

	extract($_POST);

	if($ptpnumeroata){
		$ptpnumeroata = str_replace('/','', $ptpnumeroata);
	}

	if(!$ptpnumeroata || !$ptpobjeto){
		return array("msg" => "Favor preencher todos os campos obrigatórios!");
	}

	$ptpdatainicio = formata_data_sql($ptpdatainicio);
	$ptpanoata     = $ptpano;

	$ptpobjeto = "'$ptpobjeto'";

	if($arqdsc && $arqid){
		$sql = "UPDATE
					public.arquivo
				SET
					arqdescricao = '$arqdsc'
				WHERE
					arqid = $arqid";
		$db->executar($sql);
	}

	if($_FILES['arquivo']['name']){
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$arrCampos = array();
		$file = new FilesSimec("propostatipopregao",$arrCampos,"par");
		$file->setUpload($arqdsc,"arquivo",false);
		$arqid = $file->getIdArquivo();
	}

	$arqid = !$arqid ? "null" : $arqid;

	if($ptpid){
		$sql = "UPDATE
					par.propostatipopregao
				SET
					ptpnumeroata = '$ptpnumeroata',
					ptpobjeto = $ptpobjeto,
					arqid = $arqid,
					ptpdatainicio = '$ptpdatainicio',
					ptpvigencia = $ptpvigencia,
					ptpanoata = $ptpanoata
				WHERE
					ptpid = $ptpid";
		$db->executar($sql);
	}else{
		$sql = "insert into
					par.propostatipopregao
				(ptpnumeroata,ptpobjeto,arqid, ptpdatainicio, ptpvigencia, ptpanoata)
					values
				('$ptpnumeroata',$ptpobjeto,$arqid,'$ptpdatainicio', $ptpvigencia, '$ptpanoata')
					returning ptpid";
		$ptpid = $db->pegaUm($sql);
	}

	if($ptpid){
		$sqlR = "delete from par.pregaouf where ptpid = $ptpid;";
		if($estuf){
//			$i = 0;
			foreach($estuf as $uf){
				if($uf){
					$sqlR .= "	insert into
									par.pregaouf
								(ptpid,estuf)
									values
								($ptpid,'$uf');";
				}else{
					return "erro UF";
				}
//				$i++;
			}
		}
		$db->executar($sqlR);
	}

	if($db->commit()){
		return array("msg" => "Operação realizada com sucesso!");
	}else{
		return array("msg" => "Não foi possível realizar a operação!");
	}
}

function listarPregao()
{
	global $db;

	$sql = "SELECT
				ptpid,
				'<span style=\"white-space: nowrap\" ><img style=\"cursor:pointer\" onclick=\"alterarPregao(\'' || ptpid || '\')\" src=\"/imagens/alterar.gif\" title=\"Alterar Pregão\" /> <img style=\"cursor:pointer\" onclick=\"excluirPregao(\'' || ptpid || '\')\" src=\"/imagens/excluir.gif\" title=\"Excluir Pregão\" /></span>' as acao,
				CASE WHEN LENGTH(ptpnumeroata) = 6 THEN SUBSTR (ptpnumeroata, 0, 3) ELSE SUBSTR (ptpnumeroata, 0, 4) END || '/' ||  SUBSTR (ptpnumeroata, (LENGTH(ptpnumeroata) - 3)) as nata,
			--	SUBSTR (ptpnumeroata, 0, 3) || '/' ||  SUBSTR (ptpnumeroata, 3) as nata,
				ptpobjeto,
				(CASE WHEN arqid IS NOT NULL
					THEN '<a href=\"javascript:DownloadArquivo(\'' || pre.arqid || '\')\" >' || (select arqnome || '.' || arqextensao from public.arquivo arq where arq.arqid = pre.arqid) || '</a>'
					ELSE 'N/A'
				END) as arquivo
			FROM
				par.propostatipopregao pre
			WHERE
				ptpstatus = 'A'";
	$arrDados = $db->carregar($sql);

	if($arrDados){
		$n = 0;
		foreach($arrDados as $dados){
			$sql = "select estuf from par.pregaouf where ptpid = {$dados['ptpid']}";
			$arrUF = $db->carregarColuna($sql);
			if($arrUF){
				$arrEstados = implode(", ",$arrUF);
			}else{
				$arrEstados = "N/A";
			}
			unset($dados['ptpid']);
			$arrD[$n] = $dados;
			$arrD[$n]["estados"] = $arrEstados;
			$n++;
		}
	}
	$cabecalho = array("Ação","Nº Ata","Objeto","Arquivo","Estados");
	$db->monta_lista_array($arrD,$cabecalho,100,5,"N","center");
}

function downloadArquivoPregao(){
	global $db;
	$arqid = $_REQUEST['arqid'];
	if($arqid){
		$sql ="SELECT * FROM public.arquivo WHERE arqid = ".$arqid;
			$arquivo = current($db->carregar($sql));
			$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arquivo['arqid']/1000) .'/'.$arquivo['arqid'];
		if ( !is_file( $caminho ) ) {
			die("<script>alert('Arquivo não encontrado.');history.back(-1);</script>");
		}
		if ( is_file( $caminho ) ) {
			$filename = str_replace(" ", "_", $arquivo['arqnome'].'.'.$arquivo['arqextensao']);
			header( 'Content-type: '. $arquivo['arqtipo'] );
			header( 'Content-Disposition: attachment; filename='.$filename);
			readfile( $caminho );
			exit();
		} else {
			die("<script>alert('Arquivo não encontrado.');history.back(-1);</script>");
		}
	}else{
		die("<script>alert('Arquivo não encontrado.');history.back(-1);</script>");
	}
}

function excluirArquivoPregao($arqid = null)
{
	global $db;

	$arqid = !$arqid ? $_REQUEST['arqid'] : $arqid;
	if( $arqid != '' ){
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$arrCampos = array();
		$file = new FilesSimec("propostatipopregao",$arrCampos,"par");
		$file->excluiArquivoFisico($arqid);
		$sql = "update public.arquivo set arqstatus = 'I'where arqid = $arqid";
		$db->executar($sql);
		$db->commit($sql);
	}
}

function excluirPregao()
{
	global $db;

	$ptpid = $_REQUEST['ptpid'];

	if($ptpid){

//		$arqid = $db->pegaUm("select arqid from par.propostatipopregao where ptpid = $ptpid");

//		$sql = "delete from
//					par.pregaouf
//				where
//					ptpid = $ptpid;";
		$sql.= "UPDATE par.propostatipopregao SET
					ptpstatus = 'I'
				WHERE
					ptpid = $ptpid";
		$db->executar($sql);
		$db->commit($sql);
//		if($arqid){
//			excluirArquivoPregao($arqid);
//		}
		listarPregao();
	}else{
		echo "erro";
	}
}

/*
 * FUNÇÕES PRO-FUNCIONARIO
 */
function regraEnviaPreAnalise(){

	global $db;

	$sql = "select
				pfaid
			from
				par.pfadesao
			where
				now() between pfadatainicial and pfadatafinal
			and
				prgid = {$_SESSION['par']['prgid']}";

	$validade = $db->pegaUm($sql);

	if(!$validade && $_SESSION['par']['prgid'] != PROG_PAR_ALFABETIZACAO_IDADE_CERTA){
		return 'Prazo expirado!';
	}

	//PRO FUNCIONARIO
	if($_SESSION['par']['prgid'] == 60){

		if($_SESSION['par']['muncod'])
			$msg = 'É necessário cadastrar pelo menos um tutor e um aluno em qualquer curso!';
		else
			$msg = 'É necessário cadastrar pelo menos um coordenador, um tutor e um aluno em qualquer curso!';

		//$_SESSION['par']['pfaid'] && $_SESSION['par']['inuid']
		if(!$_SESSION['par']['adpid'])
			return $msg;

		//pega total coordenador
		$sql = "select count(pfe.entid) as tot from par.pfentidade pfe
			INNER JOIN entidade.funcaoentidade efu ON efu.entid = pfe.entid AND efu.funid = ".FUNID_COORDENADOR_PRO_FUNCIONARIO."
			WHERE pfe.adpid = ". $_SESSION['par']['adpid'];
		$tot_coordenador = $db->pegaUm($sql);

		//pega total tutor
		$sql = "select count(pfe.entid) as tot from par.pfentidade pfe
			INNER JOIN entidade.funcaoentidade efu ON efu.entid = pfe.entid AND efu.funid = ".FUNID_PROF_TUTOR_PRO_FUNCIONARIO."
			WHERE pfe.adpid = ". $_SESSION['par']['adpid'];
		$tot_tutor = $db->pegaUm($sql);

		//pega total alunos
		$sql = "select count(pcuid) as tot from par.pfcursista
			WHERE adpid = ". $_SESSION['par']['adpid'];
		$tot_aluno = $db->pegaUm($sql);

		if($_SESSION['par']['muncod']){

			if($tot_tutor > 0 && $tot_aluno > 0){
				return true;
			}
			else{
				return $msg;
			}

		}
		else{

			if($tot_coordenador > 0 && $tot_tutor > 0 && $tot_aluno > 0){
				return true;
			}
			else{
				return $msg;
			}


		}

	}
	else{

		$msg = 'É necessário cadastrar pelo menos um aluno no(s) curso(s)!';

		//pega total alunos
		$sql = "select count(pcuid) as tot from par.pfcursista
			WHERE adpid = ". $_SESSION['par']['adpid'];
		$tot_aluno = $db->pegaUm($sql);

		if($tot_aluno > 0){
			return true;
		}
		else{
			return $msg;
		}

	}

}

/**
 * Controla se o interloucutor pode ser modificado
 *
 * @param integer $esdid Código do estado do documento
 * @param array $arrayPerfil Perfil do usuário
 * @return boolean Retorna TRUE caso o interloucutor possa ser modificado
 */
function verificaModificacaoFormularioInterlocutor($esdid, $arrayPerfil){
    # 1147 - Em cadastramento
    if($esdid == 1147 && (
        in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil)
        ||
        in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil)
        ||
        in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrayPerfil)
        ||
        in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrayPerfil)
        ||
        in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrayPerfil)
        ||
        in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrayPerfil)
        ||
        in_array(PAR_PERFIL_PREFEITO, $arrayPerfil)
    )){
        $resultado = TRUE;
    } else {
        $resultado = FALSE;
    }
    return $resultado;
}

/**
 * Verifica prazo Programa Adesão
 *
 * @global type $db
 * @return string/bolean Retorna texto com a mensagem de erro ou TRUE se o prazo
 * não expirou
 */
function verificarPrazoExpirado(){
    global $db;

    $sql = "
        SELECT
            pfaid
        FROM
            par.pfadesao
        WHERE
            now() between pfadatainicial
            AND pfadatafinal
            AND prgid = {$_SESSION['par']['prgid']}";

    $validade = $db->pegaUm($sql);

    # Verifica se existe adesao no intervalo do prazo do programa
    if(!$validade){
        return 'Prazo expirado!';
    }
}

/**
 * Regra de negocio para a fase passar de 'em cadastramento' para a fase seguinte
 *
 * @global type $db
 * @return string|boolean Retorna texto com a mensagem de erro ou TRUE se
 * satisfazer as validações da regra de negocio
 */
function regraPreAnaliseConselhos(){
    global $db;

    $resultadoVerificarPrazoExpirado = verificarPrazoExpirado();
    if(!empty($resultadoVerificarPrazoExpirado)){
        return $resultadoVerificarPrazoExpirado;
    }

    # Verifica se foi cadastrado um interlecutor
    $sql = "
        SELECT
            count(pcuid) AS tot
        FROM
            par.pfcursista
        WHERE adpid = ". $_SESSION['par']['adpid'];
    $qtdInterlecutor = $db->pegaUm($sql);

    if($qtdInterlecutor > 0){
        return true;
    }
    else{
        return 'É necessário cadastrar pelo menos um Interlocutor!';
    }
}


function regraPosPreAnalise(){
	global $db;


	if(!$_SESSION['par']['adpid']) return false;

	//SITUAÇÃO AUTOMÁTICA - coloca todos os registros abaixo para a situação Pré-qualificado -> pscid=1

	//PRO FUNCIONARIO
	if($_SESSION['par']['prgid'] == 60){

		//tutor
		$sql = "UPDATE par.pfentidade SET pscid=1
	 			WHERE pfeid in (
								select pfeid from par.pfentidade pfe
								INNER JOIN entidade.funcaoentidade efu ON efu.entid = pfe.entid AND efu.funid = ".FUNID_PROF_TUTOR_PRO_FUNCIONARIO."
								WHERE tfoid in (4,5,6,7,8,9,10) --Formação: 4 = Superior Completo / 5 = Superior Completo Pedagogia / 6 = Superior Completo Licenciatura / 7 = Superior Completo (outro) / 8 = Especialização / 9 = Mestrado / 10 = Doutorado
								AND tvpid in (1) --Vinculo: 1 = Concursado
								AND pffid in (3,4,8) --Função: 3 = Técnico da Secretaria Estadual de Educação / 4 = Técnico da Secretaria Municipal de Educação / 8 = Professor
								AND pfe.adpid = ". $_SESSION['par']['adpid']. "

								)";
		$db->executar($sql);


		//cursistas
		$sql = " UPDATE par.pfcursista SET pscid=1
				 WHERE pcuid in (
									--Secretaria Escolar - pfcid=1
									select pcuid from par.pfcursista
									WHERE pfcid = 1
									AND tfoid in (1,2) --Formação: 1 = Médio Incompleto / 2 = Médio Completo
									AND tvpid in (1) --Vinculo: 1 = Concursado
									AND pffid in (11) --Função: 11 - Secretário Escolar
									AND adpid = ". $_SESSION['par']['adpid'] . "

									UNION
									--MA e Manutenção de Infraestrutura Escolar - pfcid=2
									select pcuid from par.pfcursista
									WHERE pfcid = 2
									AND tfoid in (1,2) --Formação: 1 = Médio Incompleto / 2 = Médio Completo
									AND tvpid in (1) --Vinculo: 1 = Concursado
									AND pffid in (10,12,13) --Função: 10 = Fiscal de Corredor / 12 = Porteiro, Segurança / 13 = Faxineiro
									AND adpid = ". $_SESSION['par']['adpid'] . "

									UNION
									--Alimentação Escolar	- pfcid=3
									select pcuid from par.pfcursista
									WHERE pfcid = 3
									AND tfoid in (1,2) --Formação: 1 = Médio Incompleto / 2 = Médio Completo
									AND tvpid in (1) --Vinculo: 1 = Concursado
									AND pffid in (9) --Função: 9 = Merendeira
									AND adpid = ". $_SESSION['par']['adpid'] . "

									UNION
									--Multimeios Didáticos - pfcid=4
									select pcuid from par.pfcursista
									WHERE pfcid = 4
									AND tfoid in (1,2) --Formação: 1 = Médio Incompleto / 2 = Médio Completo
									AND tvpid in (1) --Vinculo: 1 = Concursado
									--AND pffid in () --Função:
									AND adpid = ". $_SESSION['par']['adpid']. "

								)";
		$db->executar($sql);



	//SITUAÇÃO AUTOMÁTICA - coloca todos os registros abaixo para a situação Não-qualificado -> pscid=2

		//tutor
		$sql = "UPDATE par.pfentidade SET pscid=2
	 			WHERE pfeid in (
								select pfeid from par.pfentidade pfe
								INNER JOIN entidade.funcaoentidade efu ON efu.entid = pfe.entid AND efu.funid = ".FUNID_PROF_TUTOR_PRO_FUNCIONARIO."
								WHERE tfoid in (1,2,3,11,12) --Formação: 1 = Médio Incompleto / 2 = Médio Completo / 3 = Superior Incompleto / 11 = Fundamental Incompleto / 12 = Fundamental Completo
								AND tvpid in (2,3,4) --Vinculo: 2 = Cedido / 3 = Contratado / 4 = Outro
								AND pffid in (1,2,5,6,7,9,10,11,12,13) --Função: 1 = Secretário Estadual de Educação / 2 = Dirigente Municipal de Educação / 5 = Diretor de Escola / 6 = Supervisor, Coordenador Pedagógico / 7 = Orientador Educacional / 9 = Merendeira / 10 = Fiscal de corredor / 11 = Secretário Escolar / 12 = Porteiro, Segurança / 13 = Faxineiro
								AND pfe.adpid = ". $_SESSION['par']['adpid']. "

								)";
		$db->executar($sql);


		//cursistas
		$sql = " UPDATE par.pfcursista SET pscid=2
				 WHERE pcuid in (
									--Secretaria Escolar - pfcid=1
									select pcuid from par.pfcursista
									WHERE pfcid = 1
									AND tfoid in (3,4,5,6,7,8,9,10,11,12) --Formação: 3 = Superior Incompleto / 4 = Superior Completo / 5 = Superior Completo Pedagogia / 6 = Superior Completo Licenciatura / 7 = Superior Completo (outro) / 8 = Especialização / 9 = Mestrado / 10 = Doutorado / 11 = Fundamental Incompleto / 12 = Fundamental Completo
									AND tvpid in (2,3,4) --Vinculo: 2 = Cedido / 3 = Contratado / 4 = Outro
									AND pffid in (1,2,3,4,5,6,7,8,9,10,12,13) --Função: 1 = Secretário Estadual de Educação / 2 = Dirigente Municipal de Educação / 3 = Técnico da Secretaria Estadual de Educação / 4 = Técnico da Secretaria Municipal de Educação / 5 = Diretor de Escola / 6 = Supervisor, Coordenador Pedagógico / 7 = Orientador Educacional / 8 = Professor / 9 = Merendeira / 10 = Fiscal de corredor / 12 = Porteiro, Segurança / 13 = Faxineiro
									AND adpid = ". $_SESSION['par']['adpid'] . "

									UNION
									--MA e Manutenção de Infraestrutura Escolar - pfcid=2
									select pcuid from par.pfcursista
									WHERE pfcid = 2
									AND tfoid in (3,4,5,6,7,8,9,10,11,12) --Formação: 3 = Superior Incompleto / 4 = Superior Completo / 5 = Superior Completo Pedagogia / 6 = Superior Completo Licenciatura / 7 = Superior Completo (outro) / 8 = Especialização / 9 = Mestrado / 10 = Doutorado / 11 = Fundamental Incompleto / 12 = Fundamental Completo
									AND tvpid in (2,3,4) --Vinculo: 2 = Cedido / 3 = Contratado / 4 = Outro
									AND pffid in (1,2,3,4,5,6,7,8,9,11) --Função: 1 = Secretário Estadual de Educação / 2 = Dirigente Municipal de Educação / 3 = Técnico da Secretaria Estadual de Educação / 4 = Técnico da Secretaria Municipal de Educação / 5 = Diretor de Escola / 6 = Supervisor, Coordenador Pedagógico / 7 = Orientador Educacional / 8 = Professor / 9 = Merendeira / 11 = Secretário Escolar
									AND adpid = ". $_SESSION['par']['adpid'] . "

									UNION
									--Alimentação Escolar	- pfcid=3
									select pcuid from par.pfcursista
									WHERE pfcid = 3
									AND tfoid in (3,4,5,6,7,8,9,10,11,12) --Formação: 3 = Superior Incompleto / 4 = Superior Completo / 5 = Superior Completo Pedagogia / 6 = Superior Completo Licenciatura / 7 = Superior Completo (outro) / 8 = Especialização / 9 = Mestrado / 10 = Doutorado / 11 = Fundamental Incompleto / 12 = Fundamental Completo
									AND tvpid in (2,3,4) --Vinculo: 2 = Cedido / 3 = Contratado / 4 = Outro
									AND pffid in (1,2,3,4,5,6,7,8,10,11,12,13) --Função: 1 = Secretário Estadual de Educação / 2 = Dirigente Municipal de Educação / 3 = Técnico da Secretaria Estadual de Educação / 4 = Técnico da Secretaria Municipal de Educação / 5 = Diretor de Escola / 6 = Supervisor, Coordenador Pedagógico / 7 = Orientador Educacional / 8 = Professor / 10 = Fiscal de corredor / 11 = Secretário Escolar / 12 = Porteiro, Segurança / 13 = Faxineiro
									AND adpid = ". $_SESSION['par']['adpid'] . "

									UNION
									--Multimeios Didáticos - pfcid=4
									select pcuid from par.pfcursista
									WHERE pfcid = 4
									AND tfoid in (3,4,5,6,7,8,9,10,11,12) --Formação: 3 = Superior Incompleto / 4 = Superior Completo / 5 = Superior Completo Pedagogia / 6 = Superior Completo Licenciatura / 7 = Superior Completo (outro) / 8 = Especialização / 9 = Mestrado / 10 = Doutorado / 11 = Fundamental Incompleto / 12 = Fundamental Completo
									AND tvpid in (2,3,4) --Vinculo: 2 = Cedido / 3 = Contratado / 4 = Outro
									--AND pffid in (1,2,3,4,5,6,7,8,9,10,11,12,13) --Função: 1 = Secretário Estadual de Educação / 2 = Dirigente Municipal de Educação / 3 = Técnico da Secretaria Estadual de Educação / 4 = Técnico da Secretaria Municipal de Educação / 5 = Diretor de Escola / 6 = Supervisor, Coordenador Pedagógico / 7 = Orientador Educacional / 8 = Professor / 9 = Merendeira / 10 = Fiscal de corredor / 11 = Secretário Escolar / 12 = Porteiro, Segurança / 13 = Faxineiro
									AND adpid = ". $_SESSION['par']['adpid']. "

								)";
		$db->executar($sql);


		$db->commit();

		regraEnviaEmail();

	}
	elseif($_SESSION['par']['prgid'] == 121){ //PROINFANTIL

		//cursistas
		$sql = " UPDATE par.pfcursista SET pscid=10 --coloca todos como não selecionado
				 WHERE pcuid in (
									--PROINFANTIL - pfcid=9
									select pcuid from par.pfcursista
									WHERE pfcid = 9
									AND adpid = ". $_SESSION['par']['adpid'] . "

								)";
		$db->executar($sql);

		$sql = " UPDATE par.pfcursista SET pscid=9
				 WHERE pcuid in (
									--PROINFANTIL - pfcid=9
									select pcuid from par.pfcursista
									WHERE pfcid = 9
									AND tfoid in (1,2,12) --Formação: 1 = Médio Incompleto / 2 = Médio Completo / 12 = Fundamental Completo
									AND tvpid in (1,3) --Vinculo: 1 = concursado / 3 = Contratado
									AND pffid in (8,15,16) --Função: 8 = Professor / 15 = Auxiliar / 16 = Monitor
									AND adpid = ". $_SESSION['par']['adpid'] . "

								)";
		$db->executar($sql);


		$db->commit();


	}

	return true;

}

function regraEnviaAnalise(){

	global $db;

	$sql = "select
				pfaid
			from
				par.pfadesao
			where
				now() between pfadatainicial and pfadatafinal
			and
				prgid = {$_SESSION['par']['prgid']}";

	$validade = $db->pegaUm($sql);

	if(!$validade && $_SESSIO['par']['prgid'] != PROG_PAR_ALFABETIZACAO_IDADE_CERTA){
		return 'Prazo expirado!';
	}

	if(!$_SESSION['par']['adpid']) return false;

	$msg = "";


	if($_SESSION['par']['prgid'] == 60){ //PRO FUNCIONARIO
		//pega total tutor
		$sql = "select count(pfe.entid) as tot from par.pfentidade pfe
			INNER JOIN entidade.funcaoentidade efu ON efu.entid = pfe.entid AND efu.funid = ".FUNID_PROF_TUTOR_PRO_FUNCIONARIO."
			WHERE pscid in (1,2) and pfe.adpid = ". $_SESSION['par']['adpid'];
		$tot_tutor = $db->pegaUm($sql);

		//pega total alunos
		$sql = "select count(pcuid) as tot from par.pfcursista
			WHERE pscid in (1,2) and adpid = ". $_SESSION['par']['adpid'];
		$tot_aluno = $db->pegaUm($sql);
	}
	else{ //OUTROS PROGRAMAS
		//pega total alunos
		$sql = "select count(pcuid) as tot from par.pfcursista
			WHERE pscid in (select pscid from par.pfsituacaocursista
							where prgid = ".$_SESSION['par']['prgid']."
							and pscstatus = 'A')
			and adpid = ". $_SESSION['par']['adpid'];
		$tot_aluno = $db->pegaUm($sql);

	}


	//PRO FUNCIONARIO
	if($_SESSION['par']['prgid'] == 60){
		if($tot_tutor == 0 || $tot_aluno == 0){
			$msg = 'É necessário cadastrar todas as novas situações dos tutores e as novas situações dos alunos em todo(s) o(s) curso(s)!';
		}
	}
	else{
		if($tot_aluno == 0){
			$msg = 'É necessário cadastrar todas as novas situações dos alunos em todo(s) o(s) curso(s)!';
		}
	}


	if($msg){
		return $msg;
	}
	else{
		return true;
	}

}

function regraEnviaMatricula(){
	global $db;

	if(!$_SESSION['par']['adpid']) return false;

	$msg = "";



	//PRO FUNCIONARIO
	if($_SESSION['par']['prgid'] == 60){
		//pega total tutor
		$sql = "select count(pfe.entid) as tot from par.pfentidade pfe
			INNER JOIN entidade.funcaoentidade efu ON efu.entid = pfe.entid AND efu.funid = ".FUNID_PROF_TUTOR_PRO_FUNCIONARIO."
			WHERE pscid in (3,4) and pfe.adpid = ". $_SESSION['par']['adpid'];
		$tot_tutor = $db->pegaUm($sql);

		//pega total alunos
		$sql = "select count(pcuid) as tot from par.pfcursista
			WHERE pscid in (3,4) and adpid = ". $_SESSION['par']['adpid'];
		$tot_aluno = $db->pegaUm($sql);

	}
	else{ //OUTROS PROGRAMAS
		//pega total alunos
		$sql = "select count(pcuid) as tot from par.pfcursista
			WHERE pscid in (select pscid from par.pfsituacaocursista
							where prgid = ".$_SESSION['par']['prgid']."
							and pscstatus = 'A')
			and adpid = ". $_SESSION['par']['adpid'];
		$tot_aluno = $db->pegaUm($sql);
	}

	//PRO FUNCIONARIO
	if($_SESSION['par']['prgid'] == 60){
		if($tot_tutor == 0 || $tot_aluno == 0){
			$msg = 'É necessário cadastrar todas as novas situações dos tutores e as novas situações dos alunos em todo(s) o(s) curso(s)!';
		}
	}
	else{
		if($tot_aluno == 0){
			$msg = 'É necessário cadastrar todas as novas situações dos alunos em todo(s) o(s) curso(s)!';
		}
	}


	if($msg){
		return $msg;
	}
	else{
		return true;
	}

}


function regraEnviaEmail(){
	global $db;

	if(!$_SESSION['par']['adpid']) return false;
	if(!$_SESSION['par']['prgid']) return false;

	$remetente = array("nome"=>SIGLA_SISTEMA. " - PAR", "email"=>$_SESSION['email_sistema']);



	//envia email para coordenador
	/*
	if(!$_SESSION['par']['muncod']){
		$sql = "select entnome, entemail from par.pfentidade pfe
				INNER JOIN entidade.funcaoentidade efu ON efu.entid = pfe.entid AND efu.funid = ".FUNID_COORDENADOR_PRO_FUNCIONARIO."
				INNER JOIN entidade.entidade ent ON ent.entid = pfe.entid
				WHERE pfe.adpid = ". $_SESSION['par']['adpid'];
		$coordenador = $db->carregar($sql);

		if($coordenador){
			foreach($coordenador as $rs){
				$destinatario = $rs['entemail'];
				if($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br"){
				  	$destinatario = $_SESSION['usuemail'];
				}

				if($destinatario){
					enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
				}
			}
		}
	}
	*/

	//envia email para tutor
	//PRO FUNCIONARIO
	if($_SESSION['par']['prgid'] == 60){
		$sql = "select ent.entnome, ent.entemail, ent2.entnome as escola,
				(CASE WHEN pscdescricao IS NULL THEN
						'Pré-análise'
					  ELSE
						pscdescricao
				END) as pscdescricao,
				esddsc
				from par.pfentidade pfe
				INNER JOIN entidade.entidade ent ON ent.entid = pfe.entid
				LEFT JOIN entidade.endereco eed ON eed.entid = ent.entid
				INNER JOIN entidade.funcaoentidade efu ON efu.entid = ent.entid AND efu.funid = ".FUNID_PROF_TUTOR_PRO_FUNCIONARIO."
				INNER JOIN entidade.funcao fun ON fun.funid = efu.funid
				LEFT JOIN entidade.funentassoc fea ON fea.fueid = efu.fueid
				LEFT JOIN entidade.entidade ent2 ON ent2.entid = fea.entid
				LEFT JOIN par.pfsituacaocursista pfs ON pfs.pscid = pfe.pscid
				LEFT JOIN par.pfadesaoprograma ap ON ap.adpid = pfe.adpid
				LEFT JOIN workflow.documento doc ON doc.docid = ap.docid
				LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
				WHERE pfe.adpid = ". $_SESSION['par']['adpid'];
		$tutor = $db->carregar($sql);
		if($tutor){
			foreach($tutor as $rs){
				$destinatario = $rs['entemail'];

				$assunto = "Cursos na fase ".$rs['esddsc'];
				$conteudo = "Prezado(a) <b>" . $rs['entnome'] . "</b> do(a) <b>".$rs['escola']."</b>,";
				$conteudo .= "<br><br><br>";
				$conteudo .= "Os cursos estão na fase de <b>".$rs['esddsc']."</b><br>";
				$conteudo .= "Sua situação como professor tutor está como: <b>" . $rs['pscdescricao'] . "</b><br><br>";
				$conteudo .= "Importante: se o seu cadastro no Profuncionário não foi qualificado, você não atendeu a algum dos critérios estabelecidos para participação no programa (a formação, o vínculo e/ou a função atualmente exercida não são compatíveis com o curso pleiteado).<br><br>";
				$conteudo .= "Em caso de dúvida, entre em contato pelo e-mail: ". $_SESSION['email_sistema'];
				$conteudo .= "<br><br><br>";
				$conteudo .= "Atenciosamente,<br> Equipe técnica do MEC - PRÓ-FUNCIONÁRIO";

				if($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br"){
			  		$emailCopia = $_SESSION['email_sistema'];
			  		$destinatario = $_SESSION['usuemail'];
			  	}

			  	if($_SERVER['HTTP_HOST'] != "simec-local" && $_SERVER['HTTP_HOST'] != "localhost"){
					if($destinatario){
						enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
					}
			  	}
			}
		}

	}

	$conteudo = $db->pegaUm("select tapconteudoemail from par.pftermoadesaoprograma where prgid=".$_SESSION['par']['prgid']);

	//envia email para alunos
	$sql = "select pcunome, pcuemail, entnome, pfcdescricao,
			(CASE WHEN pscdescricao IS NULL THEN
					'Pré-análise'
				  ELSE
					pscdescricao
			END) as pscdescricao,
			esddsc
			from par.pfcursista pfc
			INNER JOIN entidade.entidade ent ON ent.entid = pfc.entid
			LEFT JOIN par.pfsituacaocursista pfs ON pfs.pscid = pfc.pscid
			LEFT JOIN par.pfcurso pcu ON pcu.pfcid = pfc.pfcid
			LEFT JOIN par.pfadesaoprograma ap ON ap.adpid = pfc.adpid
			LEFT JOIN workflow.documento doc ON doc.docid = ap.docid
			LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
			WHERE pfc.adpid = ". $_SESSION['par']['adpid'];
	$aluno = $db->carregar($sql);
	if($aluno){
		foreach($aluno as $rs){
			$destinatario = $rs['pcuemail'];

			$assunto = "Curso ".$rs['pfcdescricao']." na fase ".$rs['esddsc'];
			$conteudo = str_replace("#ALUNO#",$rs['pcunome'],$conteudo);
			$conteudo = str_replace("#ESCOLA#",$rs['entnome'],$conteudo);
			$conteudo = str_replace("#CURSO#",$rs['pfcdescricao'],$conteudo);
			$conteudo = str_replace("#SITUACAOCURSO#",$rs['esddsc'],$conteudo);
			$conteudo = str_replace("#SITUACAOALUNO#",$rs['pscdescricao'],$conteudo);
			/*
			$conteudo = "Prezado(a) <b>" . $rs['pcunome'] . "</b> do(a) <b>".$rs['entnome']."</b>,";
			$conteudo .= "<br><br><br>";
			$conteudo .= "O curso <b>".$rs['pfcdescricao']."</b> está na fase de <b>".$rs['esddsc']."</b><br>";
			$conteudo .= "Sua situação no curso está como: <b>" . $rs['pscdescricao'] . "</b><br><br>";
			$conteudo .= "Importante: se o seu cadastro no Profuncionário não foi qualificado, você não atendeu a algum dos critérios estabelecidos para participação no programa (a formação, o vínculo e/ou a função atualmente exercida não são compatíveis com o curso pleiteado).<br><br>";
			$conteudo .= "Em caso de dúvida, entre em contato pelo e-mail: ". $_SESSION['email_sistema'];
			$conteudo .= "<br><br><br>";
			$conteudo .= "Atenciosamente,<br> Equipe técnica do MEC - PRÓ-FUNCIONÁRIO";
			*/



			if($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br"){
		  		$emailCopia = $_SESSION['email_sistema'];
		  		$destinatario = $_SESSION['usuemail'];
		  	}

		  	if($_SERVER['HTTP_HOST'] != "simec-local" && $_SERVER['HTTP_HOST'] != "localhost"){
				if($destinatario){
					enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
				}
		  	}
		}
	}

	return true;

}
/*
 * FIM FUNÇÕES PRO-FUNCIONARIO
 */


/*** FUNÇÕES WORKFLOW PAR 2010***/


function verificaBloqueioEmendaImpositiva($sbaid){
		global $db;
		$sbaid 	= (integer) $sbaid;
		$sql = "SELECT s.sbaid
				FROM par.subacao s
			WHERE
				s.frmid in (14,15)
				AND s.sbaid = ".$sbaid;
		$Eemenda = $db->pegaUm( $sql );
		if($Eemenda){
			//		@todo dia 20/07 descomentar.
			/*if( date('Y') == '2015' ){

				  return "Esta subação é do tipo de 'Emenda Impositiva' e o prazo para envio foi de até 20/07/2015";
			}*/
			if( date('Y-m-d') >= date('Y').'04-01' ){
				return 'Prazo de envio encerrado de acordo com a Portaria Interministerial n° 40/2014';
			}
		}

		return true;
}

function parPegarDocidParaEntidade($inuid){
	global $db;
	$inuid 	= (integer) $inuid;
	$sql 	= "	select docid
				from par.instrumentounidade
				where inuid = " . $inuid;

	$docid = $db->pegaUm( $sql );
	if(!$docid){
		// INSERE DOCID PARA A ENTIDADE
		$sql = "INSERT INTO workflow.documento (tpdid, esdid, docdsc, docdatainclusao)
				VALUES (44, 313, 'Em Diagnóstico', now()) returning docid ";
		$docid = $db->pegaUm($sql);

		if($docid){
			// INSERE DOCID NO ESQUEMA DO PAR
			$sql = "UPDATE par.instrumentounidade SET docid = ".$docid." WHERE inuid = ".$inuid;
			$db->executar($sql);
		}else{
			$db->rollback();
		}
		$db->commit();
	}
	return $docid;
}

function parPegarDocidParaSubacao($sbaid, $tipo = null){
	global $db;

	$sbaid 	= (integer) $sbaid;
	$sql 	= "	select docid
				from par.subacao
				where sbastatus = 'A' AND
					sbaid = " . $sbaid;

	$docid = $db->pegaUm( $sql );
	if( $docid == false ){ //if($docid === NULL){
		// INSERE DOCID PARA A ENTIDADE
		$sql = "INSERT INTO workflow.documento (tpdid, esdid, docdsc, docdatainclusao)
				VALUES (62, 451, 'Em Elaboração', now()) returning docid ";
		$docid = $db->pegaUm($sql);

		if($docid){
			// INSERE DOCID NO ESQUEMA DO PAR
			$sql = "UPDATE par.subacao SET docid = ".$docid." WHERE sbaid = ".$sbaid;
			$db->executar($sql);
		}else{
			if( $tipo != 'analise' ){
				$db->rollback();
			}
		}
		if( $tipo != 'analise' ){
			$db->commit();
		}
	}
	return $docid;
}

function parPegarArrDocidParaSubacaoObra($sbaid){
	global $db;
	$sbaid 	= (integer) $sbaid;
	$sql = "SELECT po.docid FROM par.subacaoobra so
			INNER JOIN obras.preobra po ON po.preid = so.preid
			INNER JOIN workflow.documento wd ON wd.docid = po.docid
			WHERE
				so.sobano <= date_part('year', NOW())
				AND so.sbaid = ".$sbaid;

	return $db->carregarColuna( $sql );

}

function parPegarArrDocidParaSubacaoObraEnvioAnalise($sbaid, $oPreObra = null, $oSubacaoControle = null){
	global $db;
	$sbaid 	= (integer) $sbaid;

	$sql = "SELECT
				po.preid
			FROM
				par.subacaoobra so
			INNER JOIN obras.preobra po ON po.preid = so.preid
			INNER JOIN workflow.documento wd ON wd.docid = po.docid
			WHERE
				so.sobano <= date_part('year', NOW())
				AND po.prestatus = 'A'
				AND so.sbaid = ".$sbaid;

	$preids = $db->carregarColuna( $sql );

	if( is_array($preids) ){
		$pendentes = array();
		foreach( $preids as $preid ){
			if($preid){
					//	$oPreObra 			= new PreObra();
					//	$oSubacaoControle 	= new SubacaoControle();

						$sisid = SIS_OBRAS;

						$where = "WHERE preidsistema = '{$preid}'";
						if($sisid == 23){
							$where = "WHERE preid = '{$preid}'";
						}

						//PacFNDE
						$sql = "SELECT
									pto.ptoprojetofnde
								FROM obras.preobra po
								INNER JOIN obras.pretipoobra pto ON pto.ptoid = po.ptoid
								{$where}
								AND presistema = '{$sisid}'";

						$pacFNDE = $db->pegaUm($sql);

						//Dados
//						$sql = "SELECT
//									pre.preid,
//									pre.docid,
//									pre.presistema,
//									pre.preidsistema,
//									pre.ptoid,
//									pre.preobservacao,
//									pre.prelogradouro,
//									pre.precomplemento,
//									pre.estuf,
//									pre.muncod,
//									pre.precep,
//									pre.prelatitude,
//									pre.prelongitude,
//									pre.predtinclusao,
//									pre.prebairro,
//									pre.preano,
//									pre.qrpid,
//									pre.predescricao,
//									pre.prenumero,
//									pre.pretipofundacao,
//									pre.entcodent,
//									pto.ptoclassificacaoobra,
//									pto.ptoexisteescola,
//									pto.ptodescricao,
//									pto.ptoprojetofnde,
//									mun.mundescricao,
//									pre.preidpai,
//									pre.preesfera,
//									COALESCE(oi.obrpercexec, 0) as percexec
//								FROM obras.preobra pre
//								LEFT JOIN territorios.municipio mun ON mun.muncod = pre.muncod
//								INNER JOIN ob ras.p retipoobra pto ON pto.ptoid = pre.ptoid
//								left join obr as.ob rainfraestrutura oi on oi.preid = pre.preid
//								WHERE pre.preid = '{$preid}'
//								AND pre.prestatus = 'A'";

						$sql = "SELECT
									pre.preid,
									pre.docid,
									pre.presistema,
									pre.preidsistema,
									pre.ptoid,
									pre.preobservacao,
									pre.prelogradouro,
									pre.precomplemento,
									pre.estuf,
									pre.muncod,
									pre.precep,
									pre.prelatitude,
									pre.prelongitude,
									pre.predtinclusao,
									pre.prebairro,
									pre.preano,
									pre.qrpid,
									pre.predescricao,
									pre.prenumero,
									pre.pretipofundacao,
									pre.entcodent,
									pto.ptoclassificacaoobra,
									pto.ptoexisteescola,
									pto.ptodescricao,
									pto.ptoprojetofnde,
									mun.mundescricao,
									pre.preidpai,
									pre.preesfera,
									COALESCE(oi.obrpercentultvistoria, 0) as percexec
								FROM obras.preobra pre
								INNER JOIN obras.pretipoobra 		pto ON pto.ptoid = pre.ptoid
								LEFT  JOIN territorios.municipio 	mun ON mun.muncod = pre.muncod
								LEFT  JOIN obras2.obras 			oi  ON oi.preid = pre.preid
								WHERE
									pre.preid = $preid
									AND pre.prestatus = 'A'";

						$arDados = $db->pegaLinha($sql);


						//Qrpid
						$qrpid 	= pegaQrpidPAC( $preid, 43 );


						//PAC dados
						$sql = "SELECT
									ptoid
								FROM obras.preobra
								{$where}
								AND presistema = '{$sisid}'";

						$pacDados = $db->pegaUm($sql);

						//Fotos
						$sql = "SELECT
									count(*)
								FROM
									obras.preobra pre
								INNER JOIN
									obras.preobrafotos pof on pof.preid = pre.preid
								INNER JOIN
									public.arquivo arq on arq.arqid = pof.arqid
								WHERE
									pre.preid = {$preid}
									AND pre.presistema = {$sisid}
									AND (substring(arq.arqtipo,1,5) = 'image') ";

						$pacFotos = $db->pegaUm($sql);


						// Documentos
						$stWhere = "";
						$tipoObra = $pacDados;

						if($tipoObra){
							$stWhere .= " AND tdo.ptoid = '{$tipoObra}' ";
						}

						if($tipoObra == 1){
							$stWhere .= " AND tdo.podid NOT IN (10,11,12,13,14,15,16,17) ";
						}

						$_SESSION['par']['esfera'] = $_SESSION['par']['esfera'] ? $_SESSION['par']['esfera'] : retornaEsfera( $preid );

						if($_SESSION['par']['esfera']=='M'){
							$stWhere .= " AND doc.podesfera IN ('T','M') ";
						}else{
							$stWhere .= " AND doc.podesfera IN ('T','E') ";
						}

						$sql = "select
									count(oan.arqid) as arqid,
									count(doc.podid) as podid
								from obras.pretipodocumento doc
								inner join obras.pretipodocumentoobra tdo on tdo.podid = doc.podid
								left join obras.preobraanexo oan on oan.podid = doc.podid AND oan.preid = {$preid} AND oan.poasituacao = 'A'
								left join obras.preobra pre on pre.preid = oan.preid AND presistema = {$sisid}
								WHERE doc.podstatus = 'A'
								{$stWhere}";

						$pacDocumentos =  $db->pegaLinha($sql);


						//Tipo A
						$stWhere = "";
						if($pacFNDE == 'f'){
							if($tipoObra){
								$stWhere .= " AND tdo.ptoid = '{$tipoObra}' ";
							}

							if($tipoObra == 1){
								$stWhere .= " AND tdo.podid NOT IN (1,3,4,5,6,7,8,9) ";
							}

							$_SESSION['par']['esfera'] = $_SESSION['par']['esfera'] ? $_SESSION['par']['esfera'] : retornaEsfera( $preid );

							if($_SESSION['par']['esfera']=='M'){
								$stWhere .= " AND doc.podesfera IN ('T','M') ";
							}else{
								$stWhere .= " AND doc.podesfera IN ('T','E') ";
							}

							$sql = "select
										count(oan.arqid) as arqid,
										count(doc.podid) as podid
									from obras.pretipodocumento doc
									inner join obras.pretipodocumentoobra tdo on tdo.podid = doc.podid
									left join obras.preobraanexo oan on oan.podid = doc.podid AND oan.preid = {$preid} AND oan.poasituacao = 'A'
									left join obras.preobra pre on pre.preid = oan.preid AND presistema = {$sisid}
									WHERE doc.podstatus = 'A'
									{$stWhere}";

							$pacDocumentosTipoA = $db->pegaLinha($sql);
						}

						//Questionário
						$sql = "SELECT
									count(resid)
								FROM
									questionario.resposta
								WHERE
									perid in
										( SELECT
												perid
											FROM
												questionario.pergunta
											WHERE
												grpid in
													(SELECT
															grpid
														FROM
															questionario.grupopergunta
														WHERE
															queid = 43
													)
										)
									AND qrpid = {$qrpid}";

						$pacQuestionario =  $db->pegaUm($sql);

						//Planilha Orçamentária
						$stWhere = "";
						$sistema = SIS_OBRAS;

						$stWhere = "WHERE pre.preid = '{$preid}'";

						$ptoid = $pacDados;

						if( ($ptoid == 2) || ($ptoid == 7) ){
							$stInner = " AND pre.pretipofundacao = itc.itctipofundacao ";
						}

						$sql = "SELECT
									count(itc.itcid) as itcid,
									count(ppo.ppoid) as ppoid,
									sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade) as valor,
									ptovalorminimo as minimo,
									ptovalormaximo as maximo,
									pre.ptoid
								FROM obras.preobra pre
								INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
								INNER JOIN obras.preitenscomposicao itc ON pre.ptoid = itc.ptoid AND itcquantidade > 0 {$stInner}
								LEFT JOIN obras.preplanilhaorcamentaria ppo ON itc.itcid = ppo.itcid AND ppo.preid = {$preid}
								{$stWhere}
									AND itc.itcid NOT IN (7778,8437,20249,20748)
									AND pre.presistema = '{$sistema}'
								GROUP BY pre.ptoid, ptovalorminimo, ptovalormaximo";

						$boPlanilhaOrcamentaria =  $db->pegaLinha($sql);

						//Cronograma
						$sql = "select count(*) from obras.precronograma where preid = {$preid}";

						$pacCronograma = $db->pegaUm($sql);

						//Botão
						$boPlanilhaOrcamentaria['faltam'] = $boPlanilhaOrcamentaria['itcid'] - $boPlanilhaOrcamentaria['ppoid'];

					/*	//$pacFNDE  			= $oSubacaoControle->verificaObraFNDE($preid, SIS_OBRAS);
						//$arDados  			= $oSubacaoControle->recuperarPreObra($preid);

						$qrpid 				= pegaQrpidPAC( $preid, 43 );
						$pacDados 			= $oSubacaoControle->verificaTipoObra($preid, SIS_OBRAS);
						$pacFotos 			= $oSubacaoControle->verificaFotosObra($preid, SIS_OBRAS);
						$pacDocumentos 		= $oSubacaoControle->verificaDocumentosObra($preid, SIS_OBRAS, $pacDados);
						if($pacFNDE == 'f'){
							$pacDocumentosTipoA = $oSubacaoControle->verificaDocumentosObra($preid, SIS_OBRAS, $pacDados, true);
						}
						$pacQuestionario 		= $oPreObra->verificaQuestionario($qrpid);
						$boPlanilhaOrcamentaria = $oSubacaoControle->verificaPlanilhaOrcamentaria($preid, SIS_OBRAS, $preid);
						$pacCronograma 			= $oPreObra->verificaCronograma($preid);

						$boPlanilhaOrcamentaria['faltam'] = $boPlanilhaOrcamentaria['itcid'] - $boPlanilhaOrcamentaria['ppoid'];
*/
						//Caso o ano de cadastramento da subação seja o ano de exercício é obrigatório o preenchimento de tudo.
						if( $dado['sobano'] <= date('Y') ){
							$arPendencias = array('Dados do terreno' 						   => 'Falta o preenchimento dos dados.',
											  'Relatório de vistoria' 					   => 'Falta o preenchimento dos dados do Relatório de Vistoria.',
											  'Cadastro de fotos do terreno' 			   => 'Deve conter no mínimo 3 fotos do terreno.',
											  'Cronograma físico-financeiro' 			   => 'Falta o preenchimento dos dados.',
											  'Documentos anexos' 						   => 'Falta anexar os arquivos.',
											  'Projetos - Tipo A' 						   => 'Falta anexar os arquivos.',
											  'Itens Planilha orçamentária' 			   => 'Falta(m) '.$boPlanilhaOrcamentaria['faltam'].' iten(s) a ser(em) preenchido(s) na planilha orçamentaria.',
											  'Planilha orçamentária' 					   => 'Falta(m) '.$boPlanilhaOrcamentaria['faltam'].' iten(s) a ser(em) preenchido(s) na planilha orçamentaria.',
											  'Planilha orçamentária quadra com cobertura' => 'O valor {valor} não confere, deve ser menor ou igual a R$ 490.000,00.',
											  'Planilha orçamentária Tipo B 110v' 		   => 'O valor {valor} não confere, deve estar entre R$ 1.100.000,00 e R$ 1.330.000,00.',
											  'Planilha orçamentária Tipo B 220v' 		   => 'O valor {valor} não confere, deve estar entre R$ 1.100.000,00 e R$ 1.330.000,00.',
											  'Planilha orçamentária Tipo C 110v' 		   => 'O valor {valor} não confere, deve estar entre R$ 520.000,00 e R$ 620.000,00.',
											  'Planilha orçamentária Tipo C 220v' 		   => 'O valor {valor} não confere, deve estar entre R$ 520.000,00 e R$ 620.000,00.');
						} else { //Caso os anos sejam diferentes o único preenchimento obrigatório é o do Dados do Terreno.
							$arPendencias = array('Dados do terreno' 						   => 'Falta o preenchimento dos dados.');
						}


        			$sql = "select ptoid from obras.pretipoobra where ptoprojetofnde = 'f' AND ptostatus = 'A'";
        			$arrExcTipoObra = $db->carregarColuna( $sql );
        			//$arrExcTipoObra = array(16, 9, 21, 35, 17, 18, 29, 33, 34, 30);

        			foreach($arPendencias as $k => $v){
        				if(  ( !$pacDados && $k == 'Dados do terreno' ) ||
							 ( $k == 'Relatório de vistoria' && $pacQuestionario != 22 ) ||
							 ( $pacFotos < 3 && $k == 'Cadastro de fotos do terreno' ) ||
							 ( $k == 'Itens Planilha orçamentária' && $boPlanilhaOrcamentaria['faltam'] > 0 && !in_array($pacDados, $arrExcTipoObra) ) ||
							 ( $k == 'Planilha orçamentária' && $boPlanilhaOrcamentaria['ppoid'] == 0 && $arDados['ptoprojetofnde'] == 't') ||
							 ( $k == 'Planilha orçamentária Tipo B 110v' && $boPlanilhaOrcamentaria['ptoid'] == 2 && ($boPlanilhaOrcamentaria['valor'] < 1100000 || $boPlanilhaOrcamentaria['valor'] > 1330000) ) ||
							 ( $k == 'Planilha orçamentária Tipo B 220v' && $boPlanilhaOrcamentaria['ptoid'] == 7 && ($boPlanilhaOrcamentaria['valor'] < 1100000 || $boPlanilhaOrcamentaria['valor'] > 1330000) ) ||
							 ( $k == 'Planilha orçamentária Tipo C 110v' && $boPlanilhaOrcamentaria['ptoid'] == 3 && ($boPlanilhaOrcamentaria['valor'] < 520000 || $boPlanilhaOrcamentaria['valor'] > 620000) ) ||
							 ( $k == 'Planilha orçamentária Tipo C 220v' && $boPlanilhaOrcamentaria['ptoid'] == 6 && ($boPlanilhaOrcamentaria['valor'] < 520000 || $boPlanilhaOrcamentaria['valor'] > 620000) ) ||
							 ( $k == 'Planilha orçamentária quadra com cobertura' && $boPlanilhaOrcamentaria['ptoid'] == 5 && $boPlanilhaOrcamentaria['valor'] > 490000 ) ||
							 ( $k == 'Cronograma físico-financeiro' && !$pacCronograma && $arDados['ptoprojetofnde'] == 't' ) ||
							 ( ($pacDocumentosTipoA['arqid'] != $pacDocumentosTipoA['podid'] || !$pacDocumentosTipoA) && $k == 'Projetos - Tipo A' && $arDados['ptoprojetofnde'] == 'f' ) ||
							 ( ($pacDocumentos['arqid'] != $pacDocumentos['podid'] || !$pacDocumentos) && $k == 'Documentos anexos' )
							 ){

								 switch($k){

											case 'Relatório de vistoria':
												$pendentes[] = $preid;
												break;

											case 'Cadastro de fotos do terreno':
												$pendentes[] = $preid;
												break;

											case 'Itens Planilha orçamentária':
												$pendentes[] = $preid;
												break;

											case 'Planilha orçamentária':
												$pendentes[] = $preid;
												break;

											case 'Planilha orçamentária Tipo B 110v':
												$pendentes[] = $preid;
												break;

											case 'Planilha orçamentária Tipo B 220v':
												$pendentes[] = $preid;
												break;

											case 'Planilha orçamentária Tipo C 110v':
												$pendentes[] = $preid;
												break;

											case 'Planilha orçamentária Tipo C 220v':
												$pendentes[] = $preid;
												break;
											case 'Cronograma físico-financeiro':
												$pendentes[] = $preid;
												break;
											case 'Documentos anexos':
												$pendentes[] = $preid;
												break;
										}
							 }
        				}
        			}
		}
	}

	if( $pendentes[0] ){
		$str = " AND po.preid not in (".implode(',', $pendentes).")";
	}

	$sql = "SELECT po.docid FROM par.subacaoobra so
			INNER JOIN obras.preobra po ON po.preid = so.preid
			INNER JOIN workflow.documento wd ON wd.docid = po.docid
			WHERE
				so.sobano <= date_part('year', NOW())
				AND so.sbaid = ".$sbaid." ".$str;

	return $db->carregarColuna( $sql );

}

//

function copiaplanoacao($itrid, $inuid){
	global $db;
	$strSql = '';
	$inserePlano = NULL;
	$inseridoPlano = false;

	// se existe pontuações excluir o plano de ação da entidade.
	$strSql = deletaAcoesSubacoesEntidade($itrid, $inuid, $strSql);
	if($strSql !== ""){
		if($db->executar($strSql)){
			$inserePlano = true;
		}else{
			$inserePlano = false;
		}
	}else{
		$inserePlano = true;
	}
	// se foi excluido o plano de ação atual
	if($inserePlano){
		$arracao = carregaAcoes($itrid, $inuid );
		if($arracao){
			foreach($arracao as $dados){

				// VERIFICA SE A AÇÃO JÁ EXISTE
				$sql = "SELECT aciid FROM par.acao WHERE ppaid = {$dados['ppaid']} AND ptoid = {$dados['ptoid']} AND acidsc = '{$dados['ppadsc']}' AND acistatus = 'A'";
				$ac = $db->pegaUm( $sql );

				if( !$ac ){
					// INSERE AÇÃO
					$sql = "INSERT INTO par.acao(
								ppaid,
								ptoid,
								acistatus,
								acidsc)
				    		VALUES (
								{$dados['ppaid']},
								{$dados['ptoid']},
								'A',
								'{$dados['ppadsc']}'
									) returning aciid;";

					$aciid = $db->carregar($sql);
				} else {
					$aciid[0]['aciid'] = $ac;
				}

				if($aciid){
					// recupera as subaçãoes para ser inserido.
					$arrSubacao = carregaSubacoes($itrid, $inuid, $aciid );
					if($arrSubacao){
						foreach($arrSubacao as $dados){
							// busca se a subação é exclusiva da entidade ou se é para todos.
							$listaInstrumento = listaSubacaoExcusivoParaEntidade($dados['ppsid'], $itrid, $inuid);
							//Se a subação está para alguma entidade
							if(is_array($listaInstrumento)){
								//Se a subação está para a entidade
								if(in_array($inuid,$listaInstrumento)){
									// insere subação para entidade
		 							$inseridoAcoesSubacoes = insereSubacoes($dados, $aciid);
									if($inseridoAcoesSubacoes === false){
		 								return "Ocorreu um erro ao tentar inserir o plano de ação.";
		 							}
								}
		 						// Se não existe entidade viculado insere a subação pois ela está para todos as entidades e não apenas para alguma.
							}else if($listaInstrumento === false){
								// insere subação para entidade
		 						$inseridoAcoesSubacoes = insereSubacoes($dados, $aciid);
								if($inseridoAcoesSubacoes === false){
		 							return "Ocorreu um erro ao tentar inserir o plano de ação.";
		 						}
							}
		 					if($inseridoAcoesSubacoes === false){
		 						return "Ocorreu um erro ao tentar inserir o plano de ação.";
		 					}

						}// foreach subacao
					} //if subacao

				} // if acao
			} // foreach acão
		} // if acao
			$inseridoPlano = true;
	} // if se plano excluido

	if($inseridoPlano === true){
		return true;
	}else{
		$db->rollback();
		echo "<script>alert('Não foi possivel enviar para elaboração.');</script>";
		return false;
	}
}

// VERIFICA SE 100% PONTUADO
function cemPorcentoPontuado($itrid, $inuid){
	global $db;
	$where = "";
	if($inuid != 1){ // verifica se o inuid é DF
		$where = " and i.indid not in (213, 214, 220, 267, 268, 269, 270) ";
	}else{
		if($_SESSION['par']['estuf'] != "DF"){
			$where = " and i.indid not in (213,214,220,267,268,269,270) ";
		}
	}

	//TODOS OS INDICADORES
	$sql = "SELECT count(i.indcod) as total
			FROM par.dimensao d
			INNER JOIN par.area 	 a ON a.dimid = d.dimid
			INNER JOIN par.indicador i ON i.areid = a.areid
			WHERE 	d.itrid = {$itrid}
				AND dimstatus = 'A'
				AND arestatus = 'A'
				AND indstatus = 'A'".$where;

	$indicadores = $db->pegaUm($sql);

	// TODOS OS INDICADORES QUE O MUNICÍPIO OU ESTADO PONTUOU
	$sql = "SELECT count(i.indcod) as total
			FROM par.dimensao d
			INNER JOIN par.area 	 a ON a.dimid = d.dimid
			INNER JOIN par.indicador i ON i.areid = a.areid
			INNER JOIN par.criterio  c ON c.indid = i.indid AND c.crtstatus = 'A'
			INNER JOIN par.pontuacao p ON p.crtid = c.crtid AND p.ptostatus = 'A'
			WHERE 	p.inuid = {$inuid}
				AND d.itrid = {$itrid}
				AND dimstatus = 'A'
				AND arestatus = 'A'
				AND indstatus = 'A'
				AND ptostatus = 'A'";

	$indicadoresPontuados = $db->pegaUm($sql);
	if($indicadoresPontuados >= $indicadores ){
		return true; // 100% pontuado;
	}else{
		return "Para poder Enviar para Elaboração é necessário pontuar todos os Indicadores.";
	}

}

// DELETA AÇÃO E SUBAÇÃO
function deletaAcoesSubacoesEntidade($itrid, $inuid, $strSql){
	global $db;
	// RECUPERA TODOS AS SUBAÇÕES E AÇÕES

	/*
	$sql = "SELECT ac.aciid, s.sbaid
			FROM par.dimensao d
			INNER JOIN par.area 	 a ON a.dimid = d.dimid
			INNER JOIN par.indicador i  ON i.areid = a.areid
			INNER JOIN par.criterio  c  ON c.indid = i.indid AND c.crtstatus = 'A'
			INNER JOIN par.pontuacao p  ON p.crtid = c.crtid AND p.ptostatus = 'A'
			INNER JOIN par.acao 	 ac  ON ac.ptoid = p.ptoid AND ac.acistatus = 'A'
			LEFT  JOIN par.subacao   s  ON s.aciid = ac.aciid AND s.sbastatus = 'A'
			INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
			WHERE  p.inuid = {$inuid}
			AND iu.inuid = {$inuid}
			AND d.itrid = {$itrid}
			AND dimstatus = 'A'
			AND arestatus = 'A'
			AND indstatus = 'A'
			AND ac.aciid NOT IN (SELECT
									a.aciid
								FROM
									par.acao a
								INNER JOIN par.subacao sub ON a.aciid = sub.aciid
								WHERE
									sub.sbaextraordinaria IS NOT NULL)
			AND ptostatus = 'A';";
	*/

	$sql = "SELECT ac.aciid, s.sbaid
		  	FROM par.dimensao d
		  	INNER JOIN par.area   a ON a.dimid = d.dimid
		  	INNER JOIN par.indicador i  ON i.areid = a.areid
		  	INNER JOIN par.criterio  c  ON c.indid = i.indid AND c.crtstatus = 'A'
		  	INNER JOIN par.pontuacao p  ON p.crtid = c.crtid AND p.ptostatus = 'A'
		  	INNER JOIN par.acao   ac  ON ac.ptoid = p.ptoid AND ac.acistatus = 'A'
		  	LEFT  JOIN par.subacao   s  ON s.aciid = ac.aciid AND s.sbastatus = 'A' AND s.sbaextraordinaria IS NULL AND s.sbaid NOT IN ( SELECT sbaid FROM par.empenhosubacao where eobstatus = 'A' )
		  	INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
		  	WHERE  p.inuid = {$inuid}
			AND iu.inuid = {$inuid}
			AND d.itrid = {$itrid}
		  	AND dimstatus = 'A'
		  	AND arestatus = 'A'
		  	AND indstatus = 'A'
		  	AND ptostatus = 'A'";

	$acoesSubacoes = $db->carregar($sql);

	// DELETA SUBAÇÃO
	$arrAcao = array();
	if($acoesSubacoes[0]){
		foreach($acoesSubacoes as $k =>$dados){
			//DELETANDO DADOS DA SUBAÇÃO
			if($dados['sbaid'] !== NULL){
				// DELETANDO CRONOGRAMA E PARECER
				$strSql .= "DELETE FROM par.subacaodetalhe  WHERE sbaid = {$dados['sbaid']}; ";

				// DELETANDO BENEFICIÁRIO
				$strSql .= "DELETE FROM par.subacaobeneficiario  WHERE sbaid = {$dados['sbaid']}; ";

				// VERIFICA SE A SUBAÇÃO É GLOBAL OU POR ESCOLA
				$sql = "SELECT sbacronograma FROM par.subacao  WHERE sbastatus = 'A' AND sbaid = {$dados['sbaid']}; ";
				$cronogramaSubacao = $db->pegaUm($sql);
					// RECUPERA ESCOLAS DA SUBACAO
					$sql = "SELECT sesid FROM par.subacaoescolas WHERE sbaid = {$dados['sbaid']}; ";
					$arrEscola = $db->carregar($sql);

					if(is_array($arrEscola)){
						// DELETA QTD DAS ESCOLAS DA SUBAÇÃO
						foreach($arrEscola as $itens){
							$strSql .= "DELETE FROM par.subescolas_subitenscomposicao WHERE sesid =  {$itens['sesid']}; ";
						}
					}


					// RECUPERA ITENS DE COMPOSIÇÃO
					$sql = "SELECT icoid FROM par.subacaoitenscomposicao WHERE sbaid = {$dados['sbaid']}; ";
					$arrItens = $db->carregar($sql);

					if(is_array($arrItens)){
						// DELETA QTD DAS ESCOLAS DA SUBAÇÃO
						foreach($arrItens as $itens){
							$strSql .= "DELETE FROM par.subescolas_subitenscomposicao WHERE icoid =  {$itens['icoid']}; ";
						}
					}




				// RECUPERA OBRAS
				$sql = "SELECT preid FROM par.subacaoobra WHERE sbaid = {$dados['sbaid']};  ";
				$arrObras = $db->carregar($sql);
				if(is_array($arrObras)){
					// INATIVA AS OBRAS NO PREOBRAS
					foreach($arrObras as $obras){
						$strSql .= "UPDATE obras.preobra  SET prestatus = 'I' WHERE preid = ".$obras['preid']."; ";
					}
				}

					// DELETANDO ESCOLAS
					$strSql .= "DELETE FROM par.subacaoescolas  WHERE sbaid =  {$dados['sbaid']}; ";

				// DELETANDO ITENS DE COMPOSIÇÃO
				$strSql .= "DELETE FROM par.subacaoitenscomposicao  WHERE sbaid =  {$dados['sbaid']}; ";

				// DELETANDO OBRAS
				$strSql .= "DELETE FROM par.subacaoobra  WHERE sbaid =  {$dados['sbaid']}; ";

				// DELETANDO PARECER
				$strSql .= "DELETE FROM par.subacaoparecertecnico  WHERE sbaid =  {$dados['sbaid']}; ";


			//DELETANDO SUBAÇÃO
			$strSql .= "DELETE FROM par.subacao where aciid = {$dados['aciid']} AND sbaid = {$dados['sbaid']}; ";
			}
			if(!in_array($dados['aciid'], $arrAcao)){
				$sql = "SELECT sbaid FROM par.subacao WHERE sbaextraordinaria IS NOT NULL AND aciid = ".$dados['aciid'];
				if( !$db->pegaUm( $sql ) ){
					$arrAcao[$k] = $dados['aciid'];
				}
			}
		}
	}
	//DELETANDO ACAO
	foreach($arrAcao as $dados){
		$testa = $db->pegaUm( "SELECT s.sbaid FROM par.subacao s WHERE s.aciid = {$dados} AND s.sbaid IN ( SELECT sbaid FROM par.empenhosubacao where eobstatus = 'A' )" );
		if( !$testa ){
			$strSql .= "DELETE FROM par.acao where aciid = {$dados}; ";
		}
	}

	return $strSql;
}

// Recupera para qual grupo ou município/Estado a subação está vinculada.
function listaSubacaoExcusivoParaEntidade($ppsid, $itrid){
	global $db;

	if($itrid == 1){
		// verifica se a subação está para um estado específico.
		$sql = 'SELECT iu.inuid
				FROM par.propostasubacaoestados e
				inner join par.instrumentounidade iu ON iu.estuf = e.estuf
				WHERE ppsid = '.$ppsid;
		$exclusivo =  $db->carregarColuna($sql);
	}else{
		// verifica se a subação está para um município específico.
		$sql = 'SELECT inuid
				FROM par.propostasubacaomunicipio psm
				inner join par.instrumentounidade iu ON iu.muncod = psm.muncod
				WHERE ppsid = '.$ppsid;
		$exclusivoMuncod = $db->carregarColuna($sql);

		// verifica se a subação está para um grupo de município.
		$sql = 'SELECT iu.inuid
				FROM par.propostasubacaoideb psi
				INNER JOIN territorios.tipomunicipio tm ON tm.tpmid = psi.tpmid
				INNER JOIN territorios.muntipomunicipio mtm ON mtm.tpmid = tm.tpmid
				inner join  par.instrumentounidade iu ON iu.muncod = mtm.muncod
				WHERE psi.ppsid = '.$ppsid;
		$exclusivoGrupo = $db->carregarColuna($sql);

		$exclusivo = array_merge($exclusivoMuncod, $exclusivoGrupo);
	}

	if($exclusivo[0] ){
		return $exclusivo;
	}else{
		return false;
	}
}

function carregaAcoes($itrid, $inuid ){
	global $db;
	//TODOS OS INDICADORES QUE O MUNICÍPIO OU ESTADO PONTUOU com acao e subacao
	$sql = "SELECT  p.ptoid, ac.ppaid, ac.ppadsc, iu.muncod
			FROM par.dimensao d
			INNER JOIN par.area 	 a ON a.dimid = d.dimid
			INNER JOIN par.indicador i  ON i.areid = a.areid
			INNER JOIN par.criterio  c  ON c.indid = i.indid AND c.crtstatus = 'A'
			INNER JOIN par.propostaacao ac  ON ac.crtid = c.crtid
			INNER JOIN par.pontuacao p  ON p.crtid = c.crtid AND p.ptostatus = 'A'
			INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
			WHERE   p.inuid = {$inuid}
				AND p.inuid = {$inuid}
				AND d.itrid = {$itrid}
				AND dimstatus = 'A'
				AND arestatus = 'A'
				AND indstatus = 'A'
				AND ptostatus = 'A'
			ORDER BY c.crtid;";

	$arrAcao = $db->carregar($sql);
	return $arrAcao;
}

function carregaSubacoes($itrid, $inuid, $aciid ){
	global $db;
	if(!is_array($aciid)){
		$aciid[0]['aciid'] = $aciid;
	}
	//TODOS OS INDICADORES QUE O MUNICÍPIO OU ESTADO PONTUOU com acao e subacao
	$sql = "SELECT  p.ptoid, ac.ppaid, ac.ppadsc, s.ppsdsc, s.ppsordem, s.ppsobra, s.ppsestrategiaimplementacao,
					s.ppsptres, s.ppsnaturezadespesa, s.ppsmonitora, '' as docid,
					s.frmid, s.indid, s.foaid, s.undid, s.ppsid, s.prgid, s.ptsid, iu.muncod, ac.ppaid, s.ppscronograma
			FROM par.dimensao d
			INNER JOIN par.area 	 a ON a.dimid = d.dimid
			INNER JOIN par.indicador i  ON i.areid = a.areid
			INNER JOIN par.criterio  c  ON c.indid = i.indid AND c.crtstatus = 'A'
			INNER JOIN par.propostaacao ac  ON ac.crtid = c.crtid
			INNER JOIN par.criteriopropostasubacao cp ON cp.crtid = c.crtid
			INNER JOIN par.propostasubacao   s  ON s.ppsid = cp.ppsid
			INNER JOIN par.pontuacao p  ON p.crtid = c.crtid AND p.ptostatus = 'A'
			INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
			WHERE   p.inuid = {$inuid}
				AND p.inuid = {$inuid}
				AND d.itrid = {$itrid}
				AND ac.ppaid in (select ppaid FROM par.acao WHERE acistatus = 'A' AND
					aciid = ".$aciid[0]['aciid'].")
				AND dimstatus = 'A'
				AND arestatus = 'A'
				AND indstatus = 'A'
				AND ptostatus = 'A'
			ORDER BY c.crtid;";

	$arrSubacao = $db->carregar($sql);
	return $arrSubacao;
}

function insereSubacoes($dados, $aciid){
	global $db;
	$execucao = false;
	if($dados){

		if($dados['foaid']){
			$insere['foaid'] = "foaid,";
			$values['foaid'] = $dados['foaid'].",";
		}
		if($dados['ptsid'] || $dados['ptsid'] !== NULL){
			$insere['ptsid'] = ", ptsid";
			$values['ptsid'] = " ,".$dados['ptsid'];
		}
		if($dados['undid'] == NULL){
			$dados['undid'] = "NULL";
		}
		// insere na tabela de workflow.documento

		$sql = $sql = "INSERT INTO workflow.documento (tpdid, esdid, docdsc, docdatainclusao)
				VALUES (62, ".WF_SUBACAO_ELABORACAO.", 'Em Elaboração', now()) returning docid ";
		$docidSubAcao = $db->pegaUm($sql);

		// INSERE SUBAÇÃO
		$sql = "INSERT INTO par.subacao(sbadsc, sbaordem, sbaobra, sbaestrategiaimplementacao,
	            sbaptres, sbanaturezadespesa, sbamonitoratecnico, docid, frmid,
	            indid, {$insere['foaid']} undid, ppsid, aciid, prgid, sbacronograma, sbastatus {$insere['ptsid']} )
			    VALUES ('{$dados['ppsdsc']}', {$dados['ppsordem']}, {$dados['ppsobra']}, '{$dados['ppsestrategiaimplementacao']}', {$dados['ppsptres']},
			            {$dados['ppsnaturezadespesa']}, '{$dados['ppsmonitora']}', ".$docidSubAcao.", {$dados['frmid']}, {$dados['indid']},
			            {$values['foaid']} {$dados['undid']}, {$dados['ppsid']}, {$aciid[0]['aciid']}, {$dados['prgid']}, {$dados['ppscronograma']}, 'A' {$values['ptsid']});";

		//$db->executar($sql);
		$execucao = $db->executar($sql);


		if($execucao === false){
			$db->rollback();
			return false;
		}else{
			return true;
		}
	}
}
/*** FIM FUNÇÕES WORKFLOW PAR 2010***/




function deletaAcao( $aciid, $tipoRetorno = null ){
	global $db;

	$sql = "SELECT DISTINCT a.aciid
			FROM par.acao a
			INNER JOIN par.subacao s ON s.aciid = a.aciid
			WHERE a.aciid IN ({$aciid})
				and s.sbastatus = 'A'
				and s.aciid not in (
					                select a.aciid
					                from par.empenhosubacao es
					                inner join par.empenho em on em.empid = es.empid and em.empsituacao = '2 - EFETIVADO' and eobstatus = 'A' and empstatus = 'A'
					                inner join par.subacao s on s.sbaid = es.sbaid
					                inner join par.acao a on a.aciid = s.aciid
					                WHERE a.aciid IN ($aciid)
				)";

	$ac = $db->carregarColuna( $sql );
	$acoes = implode(",", $ac);

	if($ac){
		if( $_SESSION['par']['boExcluiSubacao'] == 'S' ){
			$arrSbaid = $db->carregarColuna("select sbaid from par.subacao where aciid in ({$aciid}) AND aciid NOT IN ({$acoes})");
			foreach ($arrSbaid as $sbaid) {
				//deletaDadosSubacao($sbaid, '', false);
				$sql = "update par.subacao set sbastatus = 'I' where sbaid = $sbaid";
				$db->executar($sql);
			}
			//$sqlAcao = "DELETE FROM par.acao where aciid IN ({$aciid}) AND aciid NOT IN ({$acoes})";
			$sqlAcao = "update par.acao set acistatus = 'I' where aciid IN ({$aciid}) AND aciid NOT IN ({$acoes})";
		} else {
			$sqlAcao = "update par.acao set acistatus = 'I' where aciid IN ({$aciid}) AND aciid NOT IN ({$acoes})";
		}
	}else{
		if( $_SESSION['par']['boExcluiSubacao'] == 'S' ){
			$arrSbaid = $db->carregarColuna("select sbaid from par.subacao where aciid in ({$aciid})");
			foreach ($arrSbaid as $sbaid) {
				//deletaDadosSubacao($sbaid, '', false);
				$sql = "update par.subacao set sbastatus = 'I' where sbaid = $sbaid";
				$db->executar($sql);
			}
			//$sqlAcao = "DELETE FROM par.acao where aciid IN ({$aciid})";
			$sqlAcao = "update par.acao set acistatus = 'I' where aciid IN ({$aciid})";
		} else {
			$sqlAcao = "update par.acao set acistatus = 'I' where aciid IN ({$aciid})";
		}
	}

	if($db->executar( $sqlAcao )){
		if( $tipoRetorno == 'ANALISE' ){
			return true;
		} else {
			$db->commit();
			return true;
		}
	} else {
		$db->rollback();
		return false;
	}
}

function deletaDadosSubacao( $sbaid, $tipoRetorno = null, $boUpdateSub = false ){
	global $db;
	if($sbaid !== NULL){

		if( $boUpdateSub ){
			$db->executar("UPDATE par.subacao SET sbastatus = 'I' WHERE sbaid = $sbaid");
			$db->commit();
			return true;
		} else {
			// DELETANDO CRONOGRAMA E PARECER
			$strSql .= "DELETE FROM par.subacaodetalhe  WHERE sbaid = {$sbaid}; ";

			// DELETANDO BENEFICIÁRIO
			$strSql .= "DELETE FROM par.subacaobeneficiario  WHERE sbaid = {$sbaid}; ";

			// VERIFICA SE A SUBAÇÃO É GLOBAL OU POR ESCOLA
			$sql = "SELECT sbacronograma FROM par.subacao  WHERE sbastatus = 'A' AND sbaid = {$sbaid}; ";
			$cronogramaSubacao = $db->pegaUm($sql);

			// RECUPERA ITENS DE COMPOSIÇÃO
			$sql = "SELECT icoid FROM par.subacaoitenscomposicao WHERE sbaid = {$sbaid}; ";
			$arrItens = $db->carregar($sql);
			if(is_array($arrItens)){
				// DELETA QTD DAS ESCOLAS DA SUBAÇÃO
				foreach($arrItens as $itens){
					$strSql .= "DELETE FROM par.subescolas_subitenscomposicao WHERE icoid =  {$itens['icoid']}; ";
				}
			}

			// RECUPERA OBRAS
			$sql = "SELECT preid FROM par.subacaoobra WHERE sbaid = {$sbaid}; ";
			$arrObras = $db->carregar($sql);
			if(is_array($arrObras)){
				// INATIVA AS OBRAS NO PREOBRAS
				foreach($arrObras as $obras){
					$strSql .= "UPDATE obras.preobra  SET prestatus = 'I' WHERE preid = ".$obras['preid']."; ";
				}
			}

			// DELETANDO ESCOLAS
			$strSql .= "DELETE FROM par.subacaoescolas  WHERE sbaid =  {$sbaid}; ";

			// DELETANDO ITENS DE COMPOSIÇÃO
			$strSql .= "DELETE FROM par.subacaoitenscomposicao  WHERE sbaid =  {$sbaid}; ";

			// DELETANDO OBRAS
			$strSql .= "DELETE FROM par.subacaoobra  WHERE sbaid =  {$sbaid}; ";

			// DELETANDO PARECER
			$strSql .= "DELETE FROM par.subacaoparecertecnico  WHERE sbaid =  {$sbaid}; ";

			$strSql .= "DELETE FROM par.subacaoobravinculacao WHERE sbaid =  {$sbaid}; ";

			// DELETANDO EMPENHO
			$strSql .= "DELETE FROM par.empenhosubacao  WHERE sbaid =  {$sbaid}; ";
			$strSql .= "DELETE FROM par.empenho WHERE empid in (select empid from par.empenhosubacao where sbaid = {$sbaid} and eobstatus = 'A') and empstatus = 'A'; ";

			//DELETANDO SUBAÇÃO
			$strSql .= "DELETE FROM par.subacao where sbaid = {$sbaid}; ";

			if($db->executar( $strSql )){
				if( !$tipoRetorno ){
					$db->commit();
				}
				return true;
			} else {
				$db->rollback();
				return false;
			}
		}
	}
}

function quantidadeSubacao( $aciid ){
	global $db;
	$qtd = NULL;
	$sql = "SELECT
    			count(sbaid) as totalsubacao
			FROM par.acao a
			INNER JOIN par.subacao s ON s.aciid = a.aciid AND s.sbastatus = 'A'
			where
				a.acistatus = 'A' AND
				a.aciid = ".$aciid;

	$qtd = $db->pegaUm( $sql );

    return $qtd;
}

function excluirSubacaoArvore($sbaid){
	global $db;

    $sql = "SELECT
      			s.sbacronograma, s.ptsid, s.frmid, c.crtpontuacao, a.aciid
			FROM par.subacao s
			INNER JOIN par.acao a ON a.aciid = s.aciid AND a.acistatus = 'A'
			INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid AND p.ptostatus = 'A'
			INNER JOIN par.criterio c ON c.crtid = p.crtid AND c.crtstatus = 'A'
                  WHERE sbastatus = 'A' AND
                  		sbaid = ".$sbaid;

	$dadosSubacao = $db->pegaLinha($sql);

	if($dadosSubacao['aciid']!=''){
		$quantidade = quantidadeSubacao($dadosSubacao['aciid']);
	}

    if($dadosSubacao['crtpontuacao'] == 1 || $dadosSubacao['crtpontuacao'] == 2) {
    	//pontuacao 1 e 2

    	if( $quantidade !== 1 ){
	    	if( deletaDadosSubacao( $sbaid, '', true )){
	    		echo "Subação excluída com sucesso!";
	    	}else{
	    		echo "Falha na exclusão da subação!\nEntre em contato com o administrador do sistema!";
	    	}
    	} else {
    		echo "Falha na exclusão da subação!\nEntre em contato com o administrador do sistema!";
    	}

	} else if($dadosSubacao['crtpontuacao'] == 3 || $dadosSubacao['crtpontuacao'] == 4) {
    	//pontuação 3 e 4

    	if( deletaDadosSubacao( $sbaid, '', true )){
   			$quantidade = quantidadeSubacao($dadosSubacao['aciid']);
    		if( $quantidade != 0 ){
    			echo "Subação excluída com sucesso!";
    		} else {

    			if( deletaAcao( $dadosSubacao['aciid'] ) ){
	    			echo "Ação e subação excluídas com sucesso!";
    			} else {
		    		echo "Falha na exclusão da subação!\nEntre em contato com o administrador do sistema!";
    			}
    		}
    	}else{
    		echo "Falha na exclusão da subação!\nEntre em contato com o administrador do sistema!";
    	}
	}
}

function excluirAcaoArvore( $aciid ){
	global $db;

	$sql = "SELECT sbaid FROM par.subacao WHERE sbastatus = 'A' AND aciid = ".$aciid;
	$sbaid = $db->pegaUm( $sql );

	if( $sbaid ){
		echo "Já existem subações vinculadas a essa ação!";
	} else {
		if( deletaAcao( $aciid ) ){
	    	echo "Ação excluída com sucesso!";
    	} else {
		  	echo "Falha na exclusão da subação!\nEntre em contato com o administrador do sistema!";
    	}
	}
}


function pegarItrid( $inuid )
{
	global $db;
	$inuid = (integer) $inuid;
	$sql = "
		select
			itrid
		from cte.instrumentounidade
		where
			inuid = " . $inuid . "
	";
	return (integer) $db->pegaUm( $sql );
}

function enviaSubacaoPosAnalise( array $arrsubacao ){
	foreach($arrsubacao as $subacoes){
		$docid = parPegarDocidParaSubacao($sbaid);

	}
}

function verificaQuantidadeDiasObraPendencias( $itrid = null, $inuid ){
	global $db;

	$itrid = $itrid ? $itrid : ($db->pegaUm( "SELECT itrid FROM par.instrumentounidade WHERE inuid = ".$inuid ));

	validaSessaoPar( $inuid );
	if( $itrid == 1 || $_SESSION['par']['estuf'] == 'DF' ){
		//estado
		/*1 <=> 690
--execucao
		2 <=> 691
--praalisada
		5 <=> 763
-- licitacao
		99 <=> 689
--planejamento proponente
*/
		# ajustado para obras 2 estaduais em 06/08/2013 conforme email do Romeu por Adonias
		$sql = "SELECT distinct estado,stoid, count(1) as qtdbloqueio FROM (
					SELECT
						o.obrid,
						o.estuf AS estado,
						o.situacaoobra as stoid,
						CASE WHEN o.situacaoobra IN (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") AND desterminodeferido IS NOT NULL AND desterminodeferido >= now() THEN 0
							WHEN o.situacaoobra IN (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") AND coalesce(o.diasprimeiropagamento, o.diasinclusao) > 365 THEN 1
							ELSE 0 end as bloqueiolicitacao,
						CASE WHEN  o.situacaoobra IN (".OBR_ESDID_EM_EXECUCAO.") AND o.diasultimaalteracao > 60
							THEN 1
							ELSE 0
						END AS bloqueioexecucaoparalisada,
						CASE WHEN o.situacaoobra IN (".OBR_ESDID_PARALISADA.")
							THEN 1
							ELSE 0
						END AS bloqueioobraparalisada,
						-- planejamento pelo proponente (689) há mais de 300 dias
						CASE WHEN o.situacaoobra IN (689)
								AND
									 (
			                            CASE
			                                when htddata is not null then
			                                    case
			                                        WHEN (now() - htddata) < '24:00:00' THEN '1'
			                                        ELSE SUBSTRING((now() - htddata)::varchar,0,STRPOS((now() - htddata)::varchar,'d'))
			                                    END
			                                else
			                                    case
			                                        WHEN (now() - docdatainclusao) < '24:00:00' THEN '1'
			                                        ELSE SUBSTRING((now() - docdatainclusao)::varchar,0,STRPOS((now() - docdatainclusao)::varchar,'d'))
			                                    END
			                            END
			                        )::integer >= 300
			                        and (
										select pagvalorparcela from  obras.preobra pre
										 LEFT join par.pagamentoobra po ON po.preid   = pre.preid
										 LEFT join par.pagamento     p ON p.pagid    = po.pagid and pagstatus = 'A'
										 where pre.obrid = o.obrid
			                        ) > 0

							THEN 1
							ELSE 0
						END AS planejamentoproponenteobra

					FROM
						obras2.vm_obras_situacao_estadual o
					WHERE
						o.inuid = {$inuid}
						-- o.estuf = 'AL'
						AND o.situacaoobra in (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.", ".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.")
				) t WHERE bloqueiolicitacao = 1 OR bloqueioexecucaoparalisada = 1 OR bloqueioobraparalisada = 1 OR planejamentoproponenteobra = 1
				GROUP BY estado, stoid";

		$dados = $db->pegaLinha( $sql );
		return $dados;
	} else {
		//municipio
		$entidade = $db->pegaLinha("SELECT itrid, CASE WHEN itrid = 1 THEN estuf ELSE muncod END as entidade, CASE WHEN itrid = 1 THEN 'estuf' ELSE 'muncod' END as tipoentidade FROM par.instrumentounidade WHERE inuid = ".$inuid);
		$esfera = $entidade['tipoentidade'] == 'estuf' ? 'E' : 'M';

		$sql = "SELECT  * FROM (
					SELECT
						o.*,
						o.estuf AS estado,
						o.situacaoobra AS stoid,
						CASE WHEN o.situacaoobra IN (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") AND desterminodeferido IS NOT NULL AND desterminodeferido >= now() THEN 0
							WHEN o.situacaoobra IN (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") AND coalesce(o.diasprimeiropagamento, o.diasinclusao) > 365 THEN 1
							ELSE 0 END AS bloqueiolicitacao,
						CASE WHEN o.situacaoobra IN (".OBR_ESDID_EM_EXECUCAO.") AND o.diasultimaalteracao > 60 THEN 1 ELSE 0 END AS bloqueioexecucaoparalisada,
						CASE WHEN o.situacaoobra IN (".OBR_ESDID_PARALISADA.")
							THEN 1
							ELSE 0
						END AS bloqueioobraparalisada,
						-- planejamento pelo proponente (689) há mais de 300 dias
						CASE WHEN o.situacaoobra IN (689)
								AND
									 (
			                            CASE
			                                when htddata is not null then
			                                    case
			                                        WHEN (now() - htddata) < '24:00:00' THEN '1'
			                                        ELSE SUBSTRING((now() - htddata)::varchar,0,STRPOS((now() - htddata)::varchar,'d'))
			                                    END
			                                else
			                                    case
			                                        WHEN (now() - docdatainclusao) < '24:00:00' THEN '1'
			                                        ELSE SUBSTRING((now() - docdatainclusao)::varchar,0,STRPOS((now() - docdatainclusao)::varchar,'d'))
			                                    END
			                            END
			                        )::integer >= 300
			                        and (
										select pagvalorparcela from  obras.preobra pre
										 LEFT join par.pagamentoobra po ON po.preid   = pre.preid
										 LEFT join par.pagamento     p ON p.pagid    = po.pagid and pagstatus = 'A'
										 where pre.obrid = o.obrid
			                        ) > 0

							THEN 1
							ELSE 0
						END AS planejamentoproponenteobra,

						-- planejamento pelo proponente (689) há mais de 300 dias
						obrnome as obrdesc,
						o.muncod,
						o.estuf
					FROM
						obras2.vm_obras_situacao_municipal o
					WHERE
						o.muncod = '{$entidade['entidade']}'
						AND o.situacaoobra in (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.", ".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.")
				) t WHERE bloqueiolicitacao = 1 OR bloqueioexecucaoparalisada = 1 OR bloqueioobraparalisada = 1 OR planejamentoproponenteobra = 1
				ORDER BY bloqueiolicitacao DESC, bloqueioexecucaoparalisada DESC";

		if( !is_array($dados) || sizeof($dados) == 0 ) $dados = $db->carregar($sql);

		return $dados;
	}
}

function verificaobras( $itrid, $inuid ){

	$dadosObra = verificaQuantidadeDiasObraPendencias( $itrid, $inuid );

	if( $itrid == 1 || $_SESSION['par']['estuf'] == 'DF' ){
		if( $dadosObra['qtdbloqueio'] > 0 ){
			//não pode cadastrar a obra
			return "Você não pode executar essa operação pois existe a necessidade de atualização dos dados no sistema de monitoramento de obras.";
		} else {
			return true;
		}
	} else {

		if( is_array($dadosObra) && sizeof($dadosObra) > 0 ){
			//não pode cadastrar a obra
			return "Você não pode executar essa operação pois existe a necessidade de atualização dos dados no sistema de monitoramento de obras.";
		} else {
			return true;
		}
	}
}

function verificaQuantidadeDiasObra( $itrid = null, $inuid ){
	global $db;

	$itrid = $itrid ? $itrid : ($db->pegaUm( "SELECT itrid FROM par.instrumentounidade WHERE inuid = ".$inuid ));

	validaSessaoPar( $inuid );
	if( $itrid == 1 ){
		// necessita depara stoid = esdid
		//estado
//		$sql = "SELECT
//					est.estuf AS estado,
//					MAX( DATE_PART('days', NOW() - o.obrdtvistoria) ) AS dias
//				FROM
//					obr as.ob rainfraestrutura o
//				INNER JOIN
//					entidade.endereco e ON o.endid = e.endid
//				INNER JOIN
//					territorios.estado est ON est.estuf = e.estuf
//				INNER JOIN
//					obras.situacaoobra s ON o.stoid = s.stoid
//				INNER JOIN
//					par.instrumentounidade iu ON iu.estuf = e.estuf
//				WHERE
//					o.obsstatus = 'A'
//					AND (o.obrpercexec > 0.00 AND o.obrpercexec < 100.00)
//					AND iu.inuid = {$inuid}
//					AND o.obrdtvistoria IS NOT NULL
//					AND o.stoid not in (3,11)
//				GROUP BY
//					est.estuf";

		$sql = "SELECT
					est.estuf AS estado,
					MAX( DATE_PART('days', NOW() - o.obrdtvistoria) ) AS dias
				FROM
					obras2.obras o
				INNER JOIN workflow.documento 		doc ON doc.docid 	= o.docid
				INNER JOIN entidade.endereco 		e 	ON o.endid 		= e.endid
				INNER JOIN territorios.estado 		est ON est.estuf 	= e.estuf
				INNER JOIN par.instrumentounidade 	iu 	ON iu.estuf 	= e.estuf
				WHERE
					o.obrstatus = 'A'
					AND (o.obrpercentultvistoria > 0.00 AND o.obrpercentultvistoria < 100.00)
					AND iu.inuid = $inuid
					AND o.obrdtvistoria IS NOT NULL
					AND doc.esdid NOT IN (".OBR_ESDID_CONCLUIDA.",".OBR_ESDID_ETAPA_CONCLUIDA.")
				GROUP BY
					est.estuf";

		$dados = $db->pegaLinha( $sql );
		return $dados['dias'];
	} else {
		// necessita depara stoid = esdid
		//municipio
//		$sql = "SELECT
//					e.muncod, m.mundsc as municipio, e.estuf AS estado,
//					ent.entnome as unidade_implantadora,
//					MAX( DATE_PART('days', NOW() - o.obrdtvistoria) ) AS dias
//				FROM
//					obr as.ob rainfraestrutura o
//				INNER JOIN
//					entidade.entidade ent ON o.entidunidade = ent.entid
//				INNER JOIN
//					entidade.funcaoentidade fu on ent.entid = fu.entid
//				INNER JOIN
//					entidade.endereco e ON o.endid = e.endid
//				INNER JOIN
//					public.municipio m ON e.muncod = m.muncod
//				INNER JOIN
//					obras.situacaoobra s ON o.stoid = s.stoid
//				INNER JOIN
//					par.instrumentounidade iu ON iu.muncod = m.muncod
//				WHERE
//					o.obsstatus = 'A'
//					AND (o.obrpercexec > 0.00 AND o.obrpercexec < 100.00)
//					AND iu.inuid = {$inuid}
//					AND o.obrdtvistoria IS NOT NULL
//					AND o.stoid not in (3,11)
//					AND fu.funid = 1
//				GROUP BY
//					e.muncod,
//					m.mundsc,
//					ent.entnome,
//					e.estuf";
		$sql = "SELECT
					e.muncod, m.mundescricao as municipio, e.estuf AS estado,
					ent.entnome as unidade_implantadora,
					MAX( DATE_PART('days', NOW() - o.obrdtvistoria) ) AS dias
				FROM
					obras2.obras o
				INNER JOIN workflow.documento 		doc ON doc.docid 	= o.docid
				INNER JOIN entidade.entidade 		ent ON o.entid 		= ent.entid
				INNER JOIN entidade.funcaoentidade 	fu 	ON ent.entid 	= fu.entid
				INNER JOIN entidade.endereco 		e 	ON o.endid 		= e.endid
				INNER JOIN territorios.municipio 		m 	ON e.muncod 	= m.muncod
				INNER JOIN par.instrumentounidade 	iu 	ON iu.muncod 	= m.muncod
				WHERE
					o.obrstatus = 'A'
					AND (o.obrpercentultvistoria > 0.00 AND o.obrpercentultvistoria < 100.00)
					AND iu.inuid = $inuid
					AND o.obrdtvistoria IS NOT NULL
					AND doc.esdid NOT IN (".OBR_ESDID_CONCLUIDA.",".OBR_ESDID_ETAPA_CONCLUIDA.")
					AND fu.funid = 1
				GROUP BY
					e.muncod,
					m.mundescricao,
					ent.entnome,
					e.estuf";

		$dados = $db->pegaLinha( $sql );
		return $dados['dias'];
	}
}

/*
 * Limpa a subação por sbaid e por anos!
 *
 * O ano é um array para receber um ou vários valores. Ex: array( 0 => 2010, 1 => 2011, 2 => 2012 )
 */
function limparSubacao($sbaid, $ano = array()){
	if( $sbaid && $ano[0] ){
		//apaga as obras
		$oSubacaoobra = new SubacaoObra();
		$oSubacaoobra->deletaPorSbaid($sbaid, $ano);

		//apaga os itens de composição
		$oSubacaoitens = new SubacaoItensComposicao();
		$oSubacaoitens->deletaPorSbaid($sbaid, $ano);

		//apaga as escolas
		$oSubacaoescola = new SubacaoEscola();
		$oSubacaoescola->deletaPorSbaid($sbaid, $ano);

		//apaga os beneficiarios
		$oSubacaobeneficiario = new SubacaoBeneficiario();
		$oSubacaobeneficiario->deletaPorSbaid($sbaid, $ano);

		//apaga os detalhes da subação
		$oSubacaodetalhe = new SubacaoDetalhe();
		$oSubacaodetalhe->deletaPorSbaid($sbaid, $ano);

		$oSubacaodetalhe->commit();

		return true;
	} else {
		return false;
	}
}

function recuperaGrandesMunicipios( $inuid ){

	global $db;

	$sql = "select
				count(mu.muncod) as total
			from
				territorios.tipomunicipio mt
			left outer join territorios.muntipomunicipio mtm ON mtm.tpmid=mt.tpmid
			left outer join territorios.municipio mu ON mu.muncod=mtm.muncod
			left outer join territorios.estado es ON es.estuf=mu.estuf
			left outer join par.instrumentounidade inu on inu.muncod=mu.muncod
			where
				inu.muncod is not null and mt.gtmid = '1'
				and inu.inuid=".$inuid;

	if( $db->pegaUm( $sql ) == 0 ){
		return false;
	} else {
		return true;
	}

}


/**
 * function verificaPendenciaDeAnalise
 * Função que verifica se o plano da entidade está com alguma pendência de elaboração.
 * @param string $itrid - Se a entidade é estado ou município
 * @param string $inuid - O id da entidade
 * @return boo ou  string - Pelo $tipoRetorno indica como a função vai retornar.
 * @author Thiago Tasca Barbosa
 * @since 31/08/2011
 */
function verificaPendenciaDeAnalise($itrid, $inuid, $tipoRetorno){
	global $db;

	//VERIFICA LAT/LONG
	echo verificaLatLongEntid();

	//VERIFICA OS E-MAIL DAS SECRETARIAS/PREFEITURAS E DOS SECRETARIOS/PREFEITOS
	if($_SESSION['par']['itrid'] == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL){
		echo verificaPendenciaDadosUnidadeIntegrantesEmail();
	}

	// CARREGA TODAS AS AÇÕES E SUBAÇÕES DA ENTIDADE
	if(!$itrid){
		$itrid = $_SESSION['par']['itrid'];
	}
	if($_SESSION['par']['inuid'] != 1){ // verifica se o inuid é DF
		$where = " and i.indid not in (213, 214, 220, 267, 268, 269, 270) ";
	}
	if($_SESSION['par']['estuf'] != "DF"){
		$where = " and i.indid not in (213,214,220,267,268,269,270) ";
	}


	$sql = "SELECT
				aciid,
                acidsc,
                acinomeresponsavel,
                crtpontuacao,
                sbadsc,
                sbaid,
                frmid,
                ptsid,
                cronograma,
                quantidade,
                itens,
                beneficiario,
                escolas,
                qtdescolas,
                numeroescolas,
                tipocronograma,
                indid,
                detalhamento,
                SUM(ano2011) AS ano2011,
                SUM(ano2012) AS ano2012,
                SUM(ano2013) AS ano2013,
                SUM(ano2014) AS ano2014,
                contapreid,
                preid,
                esdid,
                sobano,
                predescricao
      		FROM (
				SELECT
					ac.aciid,
                    ac.acidsc,
                    ac.acinomeresponsavel,
                    c.crtpontuacao,
                    s.sbadsc,
                    s.sbaid,
                    s.frmid,
                    s.ptsid,
                    COUNT(sd.sbdinicio) as cronograma,
                    COUNT(sbdquantidade) as quantidade,
                    COUNT(sic.icoid) as itens,
                    COUNT(sabid) as beneficiario,
                    0 AS escolas,
                    0 AS qtdescolas,
                    0 AS numeroescolas,
                    'global' as tipocronograma,
                    i.indid,
                    count(sd.sbddetalhamento) as detalhamento,
                    CASE WHEN sd.sbdano = '2011' THEN 1  ELSE 0 END AS ano2011,
                    CASE WHEN sd.sbdano = '2012' THEN 1  ELSE 0 END AS ano2012,
                    CASE WHEN sd.sbdano = '2013' THEN 1  ELSE 0 END AS ano2013,
                    CASE WHEN sd.sbdano = '2014' THEN 1  ELSE 0 END AS ano2014,
                    count(so.preid) as contapreid,
                    so.preid,
                    doc.esdid,
                    so.sobano,
                    po.predescricao
				FROM
					par.dimensao                  		d
				INNER JOIN par.area                 	a   ON a.dimid  = d.dimid AND arestatus = 'A'
                INNER JOIN par.indicador          		i   ON i.areid  = a.areid AND indstatus = 'A'
                INNER JOIN par.criterio             	c   ON i.indid  = c.indid AND c.crtstatus = 'A'
                INNER JOIN par.pontuacao        		p   ON p.crtid  = c.crtid AND p.ptostatus = 'A'
                INNER JOIN par.acao                		ac  ON ac.ptoid = p.ptoid AND ac.acistatus = 'A'
                INNER JOIN par.subacao					s   ON s.aciid  = ac.aciid AND s.sbastatus = 'A' AND s.sbastatus = 'A'  AND s.frmid NOT IN ( 14, 15 ) --não trazer as de emenda parlamentar
                LEFT JOIN par.subacaoitenscomposicao	sic ON sic.sbaid = s.sbaid
                LEFT JOIN par.subacaodetalhe       		sd  ON sd.sbaid  = s.sbaid --AND sbdano = date_part('year', now())
                LEFT JOIN par.subacaobeneficiario    	sb  ON sb.sbaid  = s.sbaid
                LEFT JOIN par.subacaoobra so
	                INNER JOIN obras.preobra 		po  ON po.preid = so.preid  AND po.prestatus = 'A'
                	INNER JOIN workflow.documento 	doc ON doc.docid = po.docid
	                ON so.sbaid = s.sbaid
				WHERE
					d.itrid = {$itrid}
                    AND dimstatus = 'A'
                    AND p.inuid = ".$inuid."
                    AND s.sbaextraordinaria IS NULL
                    AND s.sbacronograma = 1 -- global
					AND s.sbastatus = 'A'
                    ".$where."
				GROUP BY
					ac.aciid, ac.acidsc, ac.acinomeresponsavel, c.crtpontuacao, s.sbadsc, s.sbaid, s.frmid, s.ptsid, tipocronograma, i.indid, sd.sbdano, so.preid, so.sobano, po.predescricao, doc.esdid
				UNION ALL
                SELECT
					ac.aciid,
                    ac.acidsc,
                    ac.acinomeresponsavel,
                    c.crtpontuacao,
                    s.sbadsc,
                    s.sbaid,
                    s.frmid,
                    s.ptsid,
                    COUNT(sd.sbdinicio) as cronograma,
                    COUNT(sbdquantidade) as quantidade,
                    COUNT(sic.icoid) as itens,
                    COUNT(sabid) as beneficiario,
                    escolas2.qtdescola AS escolas ,
                    (
						SELECT COUNT(se2.sesquantidade)
						FROM par.subacaoescolas se2
		                INNER JOIN par.escolas 				esc2 ON esc2.escid = se2.escid
						INNER JOIN entidade.entidade 		ent2 ON ent2.entid = esc2.entid AND ent2.entstatus = 'A' AND (ent2.entescolanova = false or ent2.entescolanova is null) and ent2.tpcid in (1,3)
						INNER JOIN entidade.funcaoentidade 	f    ON f.entid = ent2.entid AND f.funid = 3
						WHERE
							se2.sesstatus = 'A' and ent2.entcodent is not null AND
                            se2.sbaid = s.sbaid AND se2.sesstatus = 'A'
					) AS qtdescolas,
                    (
						SELECT COUNT(se2.sesid)
						FROM par.subacaoescolas se2
                    	INNER JOIN par.escolas 				esc2 ON esc2.escid = se2.escid
						INNER JOIN entidade.entidade 		ent2 ON ent2.entid = esc2.entid AND ent2.entstatus = 'A' AND (ent2.entescolanova = false OR ent2.entescolanova IS NULL) AND ent2.tpcid IN (1,3)
						INNER JOIN entidade.funcaoentidade 	f 	 ON f.entid = ent2.entid AND f.funid = 3
						WHERE
							se2.sesstatus = 'A' AND ent2.entcodent IS NOT NULL AND
                        	se2.sbaid = s.sbaid AND se2.sesstatus = 'A'
					) AS numeroescolas,
					'escola' as tipocronograma,
                    i.indid,
                    count(sd.sbddetalhamento) as detalhamento,
                    CASE WHEN sd.sbdano = '2011' THEN 1  ELSE 0 END AS ano2011,
                    CASE WHEN sd.sbdano = '2012' THEN 1  ELSE 0 END AS ano2012,
                    CASE WHEN sd.sbdano = '2013' THEN 1  ELSE 0 END AS ano2013,
                    CASE WHEN sd.sbdano = '2014' THEN 1  ELSE 0 END AS ano2014,
                    count(so.preid) as contapreid,
                    so.preid,
                    doc.esdid,
                    so.sobano,
                    po.predescricao
				FROM par.dimensao                              d
                INNER JOIN par.area                  		a  ON a.dimid = d.dimid AND arestatus = 'A'
                INNER JOIN par.indicador           			i  ON i.areid = a.areid AND indstatus = 'A'
                INNER JOIN par.criterio              		c  ON i.indid = c.indid AND c.crtstatus = 'A'
                INNER JOIN par.pontuacao         			p  ON p.crtid = c.crtid AND p.ptostatus = 'A'
                INNER JOIN par.acao                 		ac ON ac.ptoid = p.ptoid AND ac.acistatus = 'A'
                INNER JOIN par.subacao               		s  ON s.aciid = ac.aciid AND s.sbastatus = 'A' AND s.sbastatus = 'A'  AND s.frmid NOT IN ( 14, 15 ) --não trazer as de emenda parlamentar
                LEFT  JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
				LEFT  JOIN (
					SELECT se.sbaid, COUNT(sesquantidade) as qtdescola
					FROM par.subacaoescolas se
					INNER JOIN par.subacao 				s   ON s.sbaid = se.sbaid
					INNER JOIN par.acao 				a   ON a.aciid = s.aciid
					INNER JOIN par.pontuacao 			p   ON p.ptoid = a.ptoid AND p.inuid = ".$inuid."
					INNER JOIN par.escolas 				esc ON esc.escid = se.escid
					INNER JOIN entidade.entidade 		ent ON ent.entid = esc.entid
					INNER JOIN entidade.funcaoentidade 	f   ON f.entid = ent.entid AND f.funid = 3
                    WHERE
                    	se.sesstatus = 'A' AND ent.entcodent IS NOT NULL
                    	AND (ent.entescolanova = false or ent.entescolanova is null) AND ent.entstatus = 'A' and ent.tpcid in (1,3)
					GROUP BY se.sbaid
					) escolas2 on escolas2.sbaid = s.sbaid
				LEFT  JOIN par.subacaoitenscomposicao       sic ON sic.sbaid = s.sbaid
                LEFT  JOIN par.subacaobeneficiario      	sb ON sb.sbaid = s.sbaid
                LEFT  JOIN par.subacaoobra so
                	INNER JOIN obras.preobra 		po  ON po.preid = so.preid  AND po.prestatus = 'A'
                	INNER JOIN workflow.documento 	doc ON doc.docid = po.docid
                	ON so.sbaid = s.sbaid
				WHERE
					d.itrid = {$itrid}
                    AND dimstatus = 'A'
                    AND p.inuid = ".$inuid."
                    AND s.sbaextraordinaria IS NULL
                    AND s.sbaid NOT IN ( SELECT sbaid FROM par.empenhosubacao where eobstatus = 'A' )
                    AND s.sbacronograma = 2 -- por escola
					AND s.sbastatus = 'A'
                    ".$where."
             	GROUP BY
					ac.aciid, ac.acidsc, ac.acinomeresponsavel, c.crtpontuacao, s.sbadsc, s.sbaid, s.frmid, s.ptsid,
                	tipocronograma, i.indid, sd.sbdano, so.preid, so.sobano, po.predescricao,
					escolas2.qtdescola,doc.esdid
				) AS grupo
			GROUP BY
				aciid,
                acidsc,
                acinomeresponsavel,
                crtpontuacao,
                sbadsc,
                sbaid,
                frmid,
                ptsid,
                cronograma,
                quantidade,
                itens,
                beneficiario,
                escolas,
                qtdescolas,
                numeroescolas,
                tipocronograma,
                indid,
                detalhamento,
                preid,
                sobano,
                predescricao,
                contapreid,
				esdid
        	ORDER BY aciid";

// 	ver($sql, d);
	$dados = $db->carregar($sql);

	// DECLARAÇÃO DE VARIAVEIS
    $aciid 					= NULL;
    $t	 					= 0;
    $a	 					= 0;
    $erro 					= false;
    $SubacaoVazia 			= array();
    $pendenciaAcao 			= array();
    $todasAsPendencia 		= array();
    $acaoComSubacao 		= array();

    //VERIFICA PENDÊNCIAS DAS OBRAS
    $aPendencias = getObrasPendentesPAR($_SESSION['par']['muncod'], $_SESSION['par']['estuf']);

    if($aPendencias && is_array($aPendencias)){
        echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border: none;">
	    		<tr>
	    			<td colspan="2" class="TituloTela"  style="background-color:#6F8F20" >Existe a necessidade de atualização dos dados no sistema de monitoramento de obras.</td>
	    			</tr>
	    		<tr>
					<td>
                        <font color="red">';
        echo '<ul>';
        foreach ($aPendencias as $pendencia) {
            echo '<li>(' . $pendencia['obrid'] . ') - ' . $pendencia['obrnome'] . ' Motivo: ' . $pendencia['pendencia'] . '. Exec. ' . number_format($pendencia['obrpercentultvistoria'], 2, ',', '.') . '%</li>';
        }
        echo '</ul>';


        echo '          </font>
					</td>
				</tr>
	    	</table>';
    }

    // SE EXISTE AÇÕES E SUBAÇÕES
    if(is_array($dados)){
	    foreach($dados as $chave => $dado){
			if( $t > 10 || $t == 10 ){
				//break;
			}

			//ver($dados);
	    	// SE A SUBAÇÃO NÃO TEM NADA PREENCHIDO EM NENHUM DOS ANOS ENVIA A SUBAÇÃO PARA UM ARRAY PARA PODER EXCLUIR AS SUBAÇÕES.
	    	if( ($dado['cronograma']   == 0) &&
	    		($dado['quantidade']   == 0) &&
	    		($dado['itens'] 	   == 0) &&
	    		($dado['beneficiario'] == 0) &&
	    		($dado['contapreid']   == 0) &&
	    		($dado['escolas'] 	   == 0) )
	    	{
	    		$subacaoVazia[] 					= $dado['sbaid'];
	    		$subacaoVaziaAviso[$dado['sbaid']] 	= $dado['sbadsc'];
	    		$erro = true;
	    	// CASO A SUBAÇÃO NÃO ESTEJA VAZIA VERIFCA SE EXISTE ALGUMA PENDÊNCIA DE PREENCHIMENTO
	    	} else {
		    	// DE ACORDO COM O TIPO DE FORMA DE EXECUÇÃO
		    	$acaoComSubacao[] = $dado['aciid'];
		    	switch ($dado['frmid']) {
		    		// SUBACAO DO TIPO BNDES
	    			case 5:
			    		if($dado['cronograma'] == 0){
			    			if($dado['ano2013'] > 0 || $dado['anoano2011'] > 0){
				    			$erro = true;
				    			$t++;
				    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
				    			$pendenciaSubacao[$dado['sbaid']]['cronograma'] = '- O campo "cronograma físico" está vazio; <br>';
				    			$todasAsPendencia[] = $dado['sbaid'];
			    			}
			    		}
			    		if($dado['quantidade'] == 0 && $dado['tipocronograma'] == 'global'){
			    			if($dado['ano2013'] > 0 || $dado['anoano2011'] > 0){
				    			$erro = true;
				    			$t++;
				    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
				    			$pendenciaSubacao[$dado['sbaid']]['quandidade'] = '- O campo "quantidade" do cronograma está vazio; <br>';
				    			$todasAsPendencia[] = $dado['sbaid'];
			    			}
			    		}
			    		if($dado['quantidade'] == 0 && $dado['tipocronograma'] == 'escola'){
			    			$erro = true;
			    			$t++;
			    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
			    			$pendenciaSubacao[$dado['sbaid']]['quandidade'] = '- O campo "quantidade" das escolas está vazio; <br>';
			    			$todasAsPendencia[] = $dado['sbaid'];
			    		}
	        		break;
	        		// SUBACAO DO TIPO ASSISTENCIA FINANCEIRA
	        		case 6:
			    		if($dado['cronograma'] == 0){
			    			if($dado['ano2013'] > 0 || $dado['anoano2011'] > 0){
				    			$erro = true;
				    			$t++;
				    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
				    			$pendenciaSubacao[$dado['sbaid']]['cronograma'] = '- O campo "cronograma físico" está vazio; <br>';
				    			$todasAsPendencia[] = $dado['sbaid'];
			    			}
			    		}
			    		/*
			    		if($dado['beneficiario'] == 0){
			    			$erro = true;
			    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
			    			$pendenciaSubacao[$dado['sbaid']]['beneficiario'] = '- Os beneficiários não foram inseridos; <br>';
			    			$todasAsPendencia[] = $dado['sbaid'];
			    		}
						*/
			    		//SE FOR DOS TIPOS ABAIXO VALIDA CAMPO DETALHAMENTO
			    		$arrTipoSubacao = array(13, 5, 7,  18, 20);
			    		if(in_array($dado['ptsid'],$arrTipoSubacao) && $dado['detalhamento'] == 0   ){
			    			$erro = true;
			    			$t++;
			    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
			    			$pendenciaSubacao[$dado['sbaid']]['detalhamento'] = '- O campo detalhemanto é obrigatório; <br>';
			    			$todasAsPendencia[] = $dado['sbaid'];
			    		}

			    		if($dado['itens'] == 0){
			    			$erro = true;
			    			$t++;
			    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
			    			$pendenciaSubacao[$dado['sbaid']]['itens'] = '- Os itens de composição não foram inseridos; <br>';
			    			$todasAsPendencia[] = $dado['sbaid'];
			    		}
			    		if($dado['tipocronograma'] == 'escola'){
			    			if($dado['numeroescolas'] == 0){
			    				$erro = true;
				    			$t++;
			    				$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
			    				$pendenciaSubacao[$dado['sbaid']]['escolas'] = '- As escolas não foram inseridas; <br>';
			    				$todasAsPendencia[] = $dado['sbaid'];
			    			}
			    		}
	        		break;
	        		// SUBACAO DO TIPO EXECUTADA PELO MUNICÍPIO
	        		case 4:
	        			//
		    			if($dado['ptsid'] == 45 || $dado['ptsid'] == 43){ //COM ITENS
			    			if($dado['cronograma'] == 0){
			    				if($dado['ano2013'] > 0 || $dado['anoano2011'] > 0){
					    			$erro = true;
					    			$t++;
					    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
				    				$pendenciaSubacao[$dado['sbaid']]['cronograma'] = '- O campo "cronograma físico" está vazio; <br>';
					    			$todasAsPendencia[] = $dado['sbaid'];
			    				}
				    		}
				    		if($dado['quantidade'] == 0 && $dado['tipocronograma'] == 'global'){
				    			if($dado['ano2013'] > 0 || $dado['anoano2011'] > 0){
					    			$erro = true;
					    			$t++;
					    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
				    				$pendenciaSubacao[$dado['sbaid']]['quandidade'] = '- O campo "quantidade" do cronograma está vazio; <br>';
					    			$todasAsPendencia[] = $dado['sbaid'];
				    			}
				    		}
				    		if($dado['qtdescolas'] != $dado['numeroescolas'] && $dado['tipocronograma'] == 'escola'){
				    			if($dado['ano2013'] > 0 || $dado['anoano2011'] > 0){
					    			$erro = true;
					    			$t++;
					    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
					    			$pendenciaSubacao[$dado['sbaid']]['quandidade'] = '- O campo "quantidade" das escolas está vazio; <br>';
					    			$todasAsPendencia[] = $dado['sbaid'];
				    			}
				    		}
				    		if($dado['itens'] == 0){
				    			$erro = true;
				    			$t++;
				    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
			    				$pendenciaSubacao[$dado['sbaid']]['itens'] = '- Os itens de composição das subações abaixo não foram inseridos; <br>';
				    			$todasAsPendencia[] = $dado['sbaid'];
				    		}
		        		}else if($dado['ptsid'] == 46 || $dado['ptsid'] == 42){ //SEM ITENS
			        		if($dado['cronograma'] == 0){
			        			if($dado['ano2013'] > 0 || $dado['anoano2011'] > 0){
					    			$erro = true;
					    			$t++;
					    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
				    				$pendenciaSubacao[$dado['sbaid']]['cronograma'] = '- O campo "cronograma físico" está vazio; <br>';
					    			$todasAsPendencia[] = $dado['sbaid'];
			        			}
				    		}
				    		if($dado['quantidade'] == 0 && $dado['tipocronograma'] == 'global'){
				    			if($dado['ano2013'] > 0 || $dado['anoano2011'] > 0){
					    			$erro = true;
					    			$t++;
					    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
				    				$pendenciaSubacao[$dado['sbaid']]['quandidade'] = '- O campo "quantidade" do cronograma está vazio; <br>';
					    			$todasAsPendencia[] = $dado['sbaid'];
				    			}
				    		}
			        		if( ($dado['qtdescolas'] != $dado['escolas']) && $dado['tipocronograma'] == 'escola'){
				    			$erro = true;
				    			$t++;
				    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
				    			$pendenciaSubacao[$dado['sbaid']]['quandidade'] = '- O campo "quantidade" das escolas está vazio; <br>';
				    			$todasAsPendencia[] = $dado['sbaid'];
				    		}
	        			}

	        		break;
	        		// SUBACAO DO TIPO EXECUTADA PELO ESTADO
	        		case 12:
	        			// SUBAÇÕES DO TIPO COM ITENS
		    			if($dado['ptsid'] == 45){
			    			if($dado['cronograma'] == 0){
			    				if($dado['ano2013'] > 0 || $dado['anoano2011'] > 0){
					    			$erro = true;
					    			$t++;
					    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
				    				$pendenciaSubacao[$dado['sbaid']]['cronograma'] = '- O campo "cronograma físico" está vazio; <br>';
					    			$todasAsPendencia[] = $dado['sbaid'];
			    				}
				    		}
				    		if($dado['quantidade'] == 0 && $dado['tipocronograma'] == 'global'){
				    			if($dado['ano2013'] > 0 || $dado['anoano2011'] > 0){
					    			$erro = true;
					    			$t++;
					    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
				    				$pendenciaSubacao[$dado['sbaid']]['quandidade'] = '- O campo "quantidade"  do cronograma está vazio; <br>';
					    			$todasAsPendencia[] = $dado['sbaid'];
				    			}
				    		}
			    			if($dado['quantidade'] == 0 && $dado['tipocronograma'] == 'escola'){
				    			$erro = true;
				    			$t++;
				    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
				    			$pendenciaSubacao[$dado['sbaid']]['quandidade'] = '- O campo "quantidade" das escolas está vazio; <br>';
				    			$todasAsPendencia[] = $dado['sbaid'];
				    		}
				    		if($dado['itens'] == 0){
				    			$erro = true;
				    			$t++;
				    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
			    				$pendenciaSubacao[$dado['sbaid']]['itens'] = '- Os itens de composição não foram inseridos; <br>';
				    			$todasAsPendencia[] = $dado['sbaid'];
				    		}
		        		}else
		        		// SUBAÇÃOES DO TIPO SEM ITENS
		        		if($dado['ptsid'] == 46){
			        		if($dado['cronograma'] == 0){
			        			if($dado['ano2013'] > 0 || $dado['anoano2011'] > 0){
					    			$erro = true;
					    			$t++;
					    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
				    				$pendenciaSubacao[$dado['sbaid']]['cronograma'] = '- O campo "cronograma físico" está vazio; <br>';
					    			$todasAsPendencia[] = $dado['sbaid'];
			        			}
				    		}
				    		if($dado['quantidade'] == 0 && $dado['tipocronograma'] == 'global'){
				    			if($dado['ano2013'] > 0 || $dado['anoano2011'] > 0){
					    			$erro = true;
					    			$t++;
					    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
				    				$pendenciaSubacao[$dado['sbaid']]['quandidade'] = '- O campo "quantidade"  do cronograma está vazio <br>';
					    			$todasAsPendencia[] = $dado['sbaid'];
				    			}
				    		}
			        		if(($dado['qtdescolas'] != $dado['escolas']) && $dado['tipocronograma'] == 'escola'){
				    			$erro = true;
				    			$t++;
				    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
				    			$pendenciaSubacao[$dado['sbaid']]['quandidade'] = '- O campo "quantidade" das escolas está vazio; <br>';
				    			$todasAsPendencia[] = $dado['sbaid'];
				    		}
	        			}

	        		break;
	        		// ASSISTENCIA FINANCEIRA OBRAS
	        		case 11:
	        			$arrEsdid = Array(WF_PAR_EM_CADASTRAMENTO,WF_PAR_EM_DILIGENCIA);
	        			if( $dado['preid'] ){
							if( in_array($dado['esdid'],$arrEsdid) ){

		        				$preid 				= $dado['preid'];
		        				$oPreObra 			= new PreObra();
								$oSubacaoControle 	= new SubacaoControle();

								//Caso o ano de cadastramento da subação seja o ano de exercício é obrigatório o preenchimento de tudo.
								if( $dado['sobano'] <= date('Y') ){
									$pacFNDE  			= $oSubacaoControle->verificaObraFNDE($preid, SIS_OBRAS);
									$arDados  			= $oSubacaoControle->recuperarPreObra($preid);

									$qrpid 				= pegaQrpidPAC( $preid, 43 );
									$pacDados 			= $oSubacaoControle->verificaTipoObra($preid, SIS_OBRAS);
									$pacFotos 			= $oSubacaoControle->verificaFotosObra($preid, SIS_OBRAS);
									$pacDocumentos 		= $oSubacaoControle->verificaDocumentosObra($preid, SIS_OBRAS, $pacDados);
									if($pacFNDE == 'f'){
										$pacDocumentosTipoA = $oSubacaoControle->verificaDocumentosObra($preid, SIS_OBRAS, $pacDados, true);
									}
									$pacQuestionario 		= $oPreObra->verificaQuestionario($qrpid);
									$boPlanilhaOrcamentaria = $oSubacaoControle->verificaPlanilhaOrcamentaria($preid, SIS_OBRAS, $preid);
									$pacCronograma 			= $oPreObra->verificaCronograma($preid);

									$boPlanilhaOrcamentaria['faltam'] = $boPlanilhaOrcamentaria['itcid'] - $boPlanilhaOrcamentaria['ppoid'];
									$arPendencias = array('Dados do terreno' 						   => 'Falta o preenchimento dos dados.',
													  'Relatório de vistoria' 					   => 'Falta o preenchimento dos dados do Relatório de Vistoria.',
													  'Cadastro de fotos do terreno' 			   => 'Deve conter no mínimo 3 fotos do terreno.',
													  'Cronograma físico-financeiro' 			   => 'Falta o preenchimento dos dados.',
													  'Documentos anexos' 						   => 'Falta anexar os arquivos.',
													  'Projetos - Tipo A' 						   => 'Falta anexar os arquivos.',
													  'Itens Planilha orçamentária' 			   => 'Falta(m) '.$boPlanilhaOrcamentaria['faltam'].' iten(s) a ser(em) preenchido(s) na planilha orçamentaria.',
													  'Planilha orçamentária' 					   => 'Falta(m) '.$boPlanilhaOrcamentaria['faltam'].' iten(s) a ser(em) preenchido(s) na planilha orçamentaria.',
													  'Planilha orçamentária quadra com cobertura' => 'O valor {valor} não confere, deve ser menor ou igual a R$ 490.000,00.',
													  'Planilha orçamentária Tipo B 110v' 		   => 'O valor {valor} não confere, deve estar entre R$ 1.100.000,00 e R$ 1.330.000,00.',
													  'Planilha orçamentária Tipo B 220v' 		   => 'O valor {valor} não confere, deve estar entre R$ 1.100.000,00 e R$ 1.330.000,00.',
													  'Planilha orçamentária Tipo C 110v' 		   => 'O valor {valor} não confere, deve estar entre R$ 520.000,00 e R$ 620.000,00.',
													  'Planilha orçamentária Tipo C 220v' 		   => 'O valor {valor} não confere, deve estar entre R$ 520.000,00 e R$ 620.000,00.');
								} else { //Caso os anos sejam diferentes o único preenchimento obrigatório é o do Dados do Terreno.
									$pacDados 			= $oSubacaoControle->verificaTipoObra($preid, SIS_OBRAS);
									$arPendencias = array('Dados do terreno' 						   => 'Falta o preenchimento dos dados.');
								}


			        			$x=0;
			        			$sql = "select ptoid from obras.pretipoobra where ptoprojetofnde = 'f' AND ptostatus = 'A'";
			        			$arrExcTipoObra = $db->carregarColuna( $sql );
			        			//$arrExcTipoObra = array(16, 9, 21, 35, 17, 18, 29, 33, 34, 30);

								foreach($arPendencias as $k => $v){
			        				$cor = ($x % 2) ? 'white' : '#d9d9d9;';
			        				if(  ( !$pacDados && $k == 'Dados do terreno' ) ||
										 ( $k == 'Relatório de vistoria' && $pacQuestionario != 22 ) ||
										 ( $pacFotos < 3 && $k == 'Cadastro de fotos do terreno' ) ||
										 ( $k == 'Itens Planilha orçamentária' && $boPlanilhaOrcamentaria['faltam'] > 0 && !in_array($pacDados, $arrExcTipoObra) ) ||
										 ( $k == 'Planilha orçamentária' && $boPlanilhaOrcamentaria['ppoid'] == 0 && $arDados['ptoprojetofnde'] == 't') ||
										 ( $k == 'Planilha orçamentária Tipo B 110v' && $boPlanilhaOrcamentaria['ptoid'] == 2 && ($boPlanilhaOrcamentaria['valor'] < 1100000 || $boPlanilhaOrcamentaria['valor'] > 1330000) ) ||
										 ( $k == 'Planilha orçamentária Tipo B 220v' && $boPlanilhaOrcamentaria['ptoid'] == 7 && ($boPlanilhaOrcamentaria['valor'] < 1100000 || $boPlanilhaOrcamentaria['valor'] > 1330000) ) ||
										 ( $k == 'Planilha orçamentária Tipo C 110v' && $boPlanilhaOrcamentaria['ptoid'] == 3 && ($boPlanilhaOrcamentaria['valor'] < 520000 || $boPlanilhaOrcamentaria['valor'] > 620000) ) ||
										 ( $k == 'Planilha orçamentária Tipo C 220v' && $boPlanilhaOrcamentaria['ptoid'] == 6 && ($boPlanilhaOrcamentaria['valor'] < 520000 || $boPlanilhaOrcamentaria['valor'] > 620000) ) ||
										 ( $k == 'Planilha orçamentária quadra com cobertura' && $boPlanilhaOrcamentaria['ptoid'] == 5 && $boPlanilhaOrcamentaria['valor'] > 490000 ) ||
										 ( $k == 'Cronograma físico-financeiro' && !$pacCronograma && $arDados['ptoprojetofnde'] == 't' ) ||
										 ( ($pacDocumentosTipoA['arqid'] != $pacDocumentosTipoA['podid'] || !$pacDocumentosTipoA) && $k == 'Projetos - Tipo A' && $arDados['ptoprojetofnde'] == 'f' ) ||
										 ( ($pacDocumentos['arqid'] != $pacDocumentos['podid'] || !$pacDocumentos) && $k == 'Documentos anexos' )
										 ){

											 switch($k){
														case 'Dados do terreno':
															$erro = true;
											    			$t++;
															$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 		= $dado['sbadsc'];
															$pendenciaSubacao[$dado['sbaid']]['preid'][$dado['preid']]['predescricao'] 	= $dado['predescricao'];
					    									$pendenciaSubacao[$dado['sbaid']]['preid'][$dado['preid']]['dadosTerrenos'] 	= '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style="font-weight: bold;" >'.$k.'</span><br> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - '.$v.' <br>';
						    								$todasAsPendencia[] = $dado['sbaid'];
															break;

														case 'Relatório de vistoria':
															$erro = true;
											    			$t++;
															$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
															$pendenciaSubacao[$dado['sbaid']]['preid'][$dado['preid']]['predescricao'] 	= $dado['predescricao'];
					    									$pendenciaSubacao[$dado['sbaid']]['preid'][$dado['preid']]['questionario'] = '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style="font-weight: bold;" >'.$k.'</span><br> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - '.$v.' <br>';
						    								$todasAsPendencia[] = $dado['sbaid'];
															break;

														case 'Cadastro de fotos do terreno':
															$erro = true;
											    			$t++;
															$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
															$pendenciaSubacao[$dado['sbaid']]['preid'][$dado['preid']]['predescricao'] 	= $dado['predescricao'];
					    									$pendenciaSubacao[$dado['sbaid']]['preid'][$dado['preid']]['fotos'] = '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style="font-weight: bold;" >'.$k.'</span><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - '.$v.' <br>';
						    								$todasAsPendencia[] = $dado['sbaid'];
															break;

														case 'Itens Planilha orçamentária':
															$erro = true;
											    			$t++;
															$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
															$pendenciaSubacao[$dado['sbaid']]['preid'][$dado['preid']]['predescricao'] 	= $dado['predescricao'];
					    									$pendenciaSubacao[$dado['sbaid']]['preid'][$dado['preid']]['itensplanilhaOrcamentaria'] = '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style="font-weight: bold;" >'.$k.'</span><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - '.$v.' <br>';
						    								$todasAsPendencia[] = $dado['sbaid'];
															break;

														case 'Planilha orçamentária':
															$erro = true;
											    			$t++;
															$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
															$pendenciaSubacao[$dado['sbaid']]['preid'][$dado['preid']]['predescricao'] 	= $dado['predescricao'];
					    									$pendenciaSubacao[$dado['sbaid']]['preid'][$dado['preid']]['planilhaOrcamentaria'] = '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style="font-weight: bold;" >'.$k.'</span><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - '.$v.' <br>';
						    								$todasAsPendencia[] = $dado['sbaid'];
															break;

														case 'Planilha orçamentária Tipo B 110v':
															$erro = true;
											    			$t++;
															$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
															$pendenciaSubacao[$dado['sbaid']]['preid'][$dado['preid']]['predescricao'] 	= $dado['predescricao'];
					    									$pendenciaSubacao[$dado['sbaid']]['preid'][$dado['preid']]['planilhaOrcamentariaTipoB110'] = '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style="font-weight: bold;" >'.$k.'</span><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - '.$v.' <br>';
						    								$todasAsPendencia[] = $dado['sbaid'];
															break;

														case 'Planilha orçamentária Tipo B 220v':
															$erro = true;
											    			$t++;
															$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
															$pendenciaSubacao[$dado['sbaid']]['preid'][$dado['preid']]['predescricao'] 	= $dado['predescricao'];
					    									$pendenciaSubacao[$dado['sbaid']]['preid'][$dado['preid']]['planilhaOrcamentariaB220'] = '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style="font-weight: bold;" >'.$k.'</span><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - '.$v.' <br>';
						    								$todasAsPendencia[] = $dado['sbaid'];
															break;

														case 'Planilha orçamentária Tipo C 110v':
															$erro = true;
											    			$t++;
															$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
															$pendenciaSubacao[$dado['sbaid']]['preid'][$dado['preid']]['predescricao'] 	= $dado['predescricao'];
					    									$pendenciaSubacao[$dado['sbaid']]['preid'][$dado['preid']]['planilhaOrcamentariaC110'] = '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style="font-weight: bold;" >'.$k.'</span><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - '.$v.' <br>';
						    								$todasAsPendencia[] = $dado['sbaid'];
															break;

														case 'Planilha orçamentária Tipo C 220v':
															$erro = true;
											    			$t++;
															$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
															$pendenciaSubacao[$dado['sbaid']]['preid'][$dado['preid']]['predescricao'] 	= $dado['predescricao'];
					    									$pendenciaSubacao[$dado['sbaid']]['preid'][$dado['preid']]['planilhaOrcamentariaC220'] = '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style="font-weight: bold;" >'.$k.'</span><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - '.$v.' <br>';
						    								$todasAsPendencia[] = $dado['sbaid'];
															break;
														case 'Cronograma físico-financeiro':
															$erro = true;
											    			$t++;
															$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
															$pendenciaSubacao[$dado['sbaid']]['preid'][$dado['preid']]['predescricao'] 	= $dado['predescricao'];
					    									$pendenciaSubacao[$dado['sbaid']]['preid'][$dado['preid']]['cronogramaFisicoFinanceiro'] = '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style="font-weight: bold;" >'.$k.'</span><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - '.$v.' <br>';
						    								$todasAsPendencia[] = $dado['sbaid'];
															break;
														case 'Documentos anexos':
															$erro = true;
											    			$t++;
															$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
															$pendenciaSubacao[$dado['sbaid']]['preid'][$dado['preid']]['predescricao'] 	= $dado['predescricao'];
					    									$pendenciaSubacao[$dado['sbaid']]['preid'][$dado['preid']]['documentosAnexos'] = '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style="font-weight: bold;" >'.$k.'</span><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - '.$v.' <br>';
						    								$todasAsPendencia[] = $dado['sbaid'];
															break;
											}
									 	$x++;
									 }
		        				}
		        			}
	        			}else{
	        				$erro = true;
			    			$t++;
			    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
			    			$pendenciaSubacao[$dado['sbaid']]['obra'] = '- Não existe obras cadastradas; <br>';
			    			$todasAsPendencia[] = $dado['sbaid'];
	        			}
	        		break;

	        		// ASSISTENCIA TECNICA
	        		case 2:
			    		if($dado['cronograma'] == 0){
			    			$erro = true;
			    			$t++;
			    			$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
			    			$pendenciaSubacao[$dado['sbaid']]['cronograma'] = '- O campo "cronograma físico" está vazio; <br>';
			    			$todasAsPendencia[] = $dado['sbaid'];
			    		}
			    		if($dado['tipocronograma'] == 'escola'){
			    			if($dado['numeroescolas'] == 0){
			    				$erro = true;
				    			$t++;
			    				$pendenciaSubacao[$dado['sbaid']]['sbadsc'] 	= $dado['sbadsc'];
			    				$pendenciaSubacao[$dado['sbaid']]['escolas'] = '- As escolas não foram inseridas; <br>';
			    				$todasAsPendencia[] = $dado['sbaid'];
			    			}
			    		}
	        		break;
		    	} // fim switch
	    	} // fim if
	    } // fim foreach

	    // VERIFCA AÇÕES DA ENTIDADE
	    foreach($dados as $chave => $dado){
	    	if( $a > 10 || $a == 10 ){
				break;
			}
	    	// PARA NÃO ANALISAR AÇÕES REPETIDAS USA O IF
	    	if($dado['aciid'] != $aciid){
	    		// se a pontuação for um ou dois a ação tem que estar preenchida.
	    		if($dado['crtpontuacao'] == 1 || $dado['crtpontuacao'] == 2 ){
	    			if($dado['acinomeresponsavel'] == NULL){
	    				$pendenciaAcao[$dado['aciid']]['aciid'] = $dado['acidsc'];
	    				$pendenciaAcao[$dado['aciid']]['indid'] = $dado['indid'];
	    				$pendenciaAcao[$dado['aciid']]['possui'] = true;
	    				$erro = true;
	    				$a++;
	    			}
	    			if( !in_array( $dado['aciid'], $acaoComSubacao ) ){
	    				$erro = true;
	    				$pendenciaAcao[$dado['aciid']]['aciid'] 		 = $dado['acidsc'];
	    				$pendenciaAcao[$dado['aciid']]['indid'] 		 = $dado['indid'];
	    				$pendenciaAcao[$dado['aciid']]['acaoSemSubacao'] = $dado['acidsc'];
	    			}
	    		}else
	    		// Obrigar preenchimento das ações de pontuação 3  ou 4, se houver alguma subação preenchida
	    		if ($dado['crtpontuacao'] == 3 || $dado['crtpontuacao'] == 4){
		    		$subacaoVazia = $subacaoVazia ? $subacaoVazia : array();
	    			if(!in_array($dado['sbaid'],$todasAsPendencia) && !in_array($dado['sbaid'],$subacaoVazia)){
	    				if($dado['acinomeresponsavel'] == NULL){
		    				$pendenciaAcao[$dado['aciid']]['aciid'] = $dado['acidsc'];
		    				$pendenciaAcao[$dado['aciid']]['indid'] = $dado['indid'];
		    				$pendenciaAcao[$dado['aciid']]['possui'] = true;
		    				$erro = true;
		    				$a++;
	    				}
	    			}
	    		}
	    		$aciid = $dado['aciid'];
	    	}
	    }
    } // fim se array

    // VERIFICA SE A ENTIDADE PONTUOU TODO O PLANO DE AÇÃO
    $cemPorcentoPontuado 	= cemPorcentoPontuado($itrid, $inuid);
    $erroPontuado 			= false;
    $teste					= 0;
    if($cemPorcentoPontuado == false)
    {
    	$erro 			= true;
    	$erroPontuado 	= true;
    }

    // CASO EXISTE ALGUM ERRO NAS VERIFICAÇÕES ACIMA ELE TRATA O ERRO PARA MOSTRAR EM TELA.
    if($erro){
    	// MOSTRA ERRO DE PONTUAÇÃO
    	if($erroPontuado){
    	$htmlPontuado = '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border: none;">
    						<tr>
    							<td colspan="2" class="SubTituloEsquerda" style="background-color:#CE1818">É necessário pontuar todos indicadores.</td>
    						</tr>
    					 </table>
    					 <br>';
    	}

    	// MOSTRA ERRO DE AÇÃO
    	if(count($pendenciaAcao)){
    		$teste = 1;
    		if( $a > 10 || $a == 10 ){
    			//$str = " (Exibindo apenas as 10 primeiras pendências)";
    		}
    		$html .= '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border: none;">
    					<tr>
    						<td colspan="2" class="TituloTela"  style="background-color:#6F8F20" >Existem Ações com Pendências de preenchimento</td>
    					</tr>';

	    	$i = 0;
    		$c = 0;
	    	foreach($pendenciaAcao as $chave=>$pendencias){
    			// TRATAMENTO DE COR NAS LINHAS
	    		if($pendencias['possui']){
	    			if($i == 0){
						$bgcolor	= "#E9E9E9";			$i	= 1;		}
					else{
						$bgcolor	= "#FCFCFC";			$i	= 0;		}

					if( $c == 0 ){
						$html .= '<tr>
		    						<td colspan="2" class="SubTituloEsquerda" style="background-color:#A3BF5B">Necessário preencher todos os campos do formulário das ações abaixo'.$str.':</td>
		    					</tr>';
					}

	    			$html .='<tr>
	    						<td style="background-color:'.$bgcolor.' " >
	    							<img border="0" onclick="visualizarAcao('.$chave.', '.$pendencias['indid'].')" src="../imagens/consultar.gif" align="absmiddle" style="cursor:pointer;" title="Visualizar Subação" />
	    						</td>
								<td style="background-color:'.$bgcolor.' ">'.$pendencias["aciid"].'</td>
							 </tr>';
	    			$c++;
	    		}
    		}
    		$c = 0;
	    	foreach($pendenciaAcao as $chave=>$pendencias){
    			// TRATAMENTO DE COR NAS LINHAS
    			if($pendencias['acaoSemSubacao']){
	    			if($i == 0){
						$bgcolor	= "#E9E9E9";			$i	= 1;		}
					else{
						$bgcolor	= "#FCFCFC";			$i	= 0;		}

					if( $c == 0 ){
						$html .= '<tr>
		    						<td colspan="2" class="SubTituloEsquerda" style="background-color:#A3BF5B">Necessário preencher pelo menos uma subação das ações abaixo'.$str.':</td>
		    					</tr>';
					}

	    			$html .='<tr>
	    						<td style="background-color:'.$bgcolor.' " >
	    							<img border="0" onclick="visualizarAcao('.$chave.', '.$pendencias['indid'].')" src="../imagens/consultar.gif" align="absmiddle" style="cursor:pointer;" title="Visualizar Subação" />
	    						</td>
								<td style="background-color:'.$bgcolor.' ">'.$pendencias["aciid"].'</td>
							 </tr>';
	    			$c++;
    			}
    		}
    		$html .='</table> <br>';
    	}

    	// MOSTRA ERRO DAS SUBAÇÕES
    	if(count($todasAsPendencia) > 0){
    		$teste = 1;
    		$html .= '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border: none;">
    					<tr>
    						<td colspan="2" class="TituloTela"  style="background-color:#6F8F20" >Existem Subações com Pendências de preenchimento</td>
    					</tr>';
    	}

    	// SE EXISTE PENDÊNICA EM UMA SUBAÇÃO MONTA A LISTA DE PENDÊNCIAS.
    	if(count($pendenciaSubacao) > 0){
    		$teste = 1;
    		if( $t > 10 || $t == 10 ){
    		//	$str2 = " (Exibindo apenas as 10 primeiras pendências)";
    		}
    		$html .= '<tr>
    					<td colspan="2" class="SubTituloEsquerda" style="background-color:#A3BF5B">
							É necessário preencher todos os campos do formulário das subações abaixo'.$str2.':
    					</td>
    				  </tr>';
    		//dbg($pendenciaSubacao);
    		foreach($pendenciaSubacao as $sbaid=>$valor){
    			$html .= '
    					<tr>
    						<td colspan="2"  style="background-color:#BFBCB7 " >
    							<img border="0" onclick="visualizarSubacao('.$sbaid.')" src="../imagens/consultar.gif" align="absmiddle" style="cursor:pointer;" title="Visualizar Subação" />
    						'.$valor["sbadsc"].'
    						</td>
    					</tr>
    					<tr>
    						<td style="background-color:'.$bgcolor.' ">
    						</td>
    						<td style="background-color:'.$bgcolor.' ">'
    							.$valor["quandidade"]
    							.$valor["cronograma"]
    							.$valor["beneficiario"]
    							.$valor["escolas"]
    							.$valor["detalhamento"]
    							.$valor["obra"]
    							.$valor["itens"].'';

    							if($valor['preid']){
    								$obras = $valor['preid'];
    								foreach($obras as $dadosObra){
    									$html .=	'<span style="font-weight: bold;" >A obra: '.$dadosObra['predescricao'].' está com pendências abaixo: </span><br>'
			    									.$dadosObra["dadosTerrenos"]
					    							.$dadosObra["questionario"]
					    							.$dadosObra["fotos"]
					    							.$dadosObra["itensplanilhaOrcamentaria"]
					    							.$dadosObra["planilhaOrcamentaria"]
					    							.$dadosObra["planilhaOrcamentariaTipoB110"]
					    							.$dadosObra["planilhaOrcamentariaB220"]
					    							.$dadosObra["planilhaOrcamentariaC110"]
					    							.$dadosObra["planilhaOrcamentariaC220"]
					    							.$dadosObra["documentosAnexos"]
					    							.$dadosObra["cronogramaFisicoFinanceiro"].'<br>';

    								}
    							}
    							/*

    							*/


    			$html .= '
    						</td>
    				    </tr>';
    		}
    	}

    	// MOSTRAS A LISTA DE SUBAÇÕES QUE SERÃO APAGADAS PELO SISTEMA.
    	if(count($subacaoVazia) >= 1){
    		//vai verificar a distribuição do preenchimento das subações!
    		if( $html == "" ){
    			$html .= verificaDistribuicaoSubacoes($itrid);
    		}
    	//	if($html == ""){
    			$html .= '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border: none;">';
    //		}
    		$html .= '<tr>
    						<td colspan="2" class="SubTituloEsquerda" style="background-color:#D11717;  color: white;" >
    							As subações abaixo não foram preenchidas. Por esse motivo serão apagadas pelo sistema no momento do envio do PAR para análise do MEC.
    						</td>
    				    </tr>';
    		$i = 0;
    		foreach($subacaoVaziaAviso as $chave=>$pendencias){
    			// TRATAMENTO DE COR NAS LINHAS
    			if($i == 0){
					$bgcolor	= "#E9E9E9";			$i	= 1;		}
				else{
					$bgcolor	= "#FCFCFC";			$i	= 0;		}
    			$html .='<tr>
    						<td style="background-color:'.$bgcolor.' " >
    							<img border="0" onclick="visualizarSubacao('.$chave.')" src="../imagens/consultar.gif" align="absmiddle" style="cursor:pointer;" title="Visualizar Subação" />
    						</td>
							<td style="background-color:'.$bgcolor.' ">'.$pendencias.'</td>
						 </tr>';
    		}
    	}

    	if(count($todasAsPendencia) > 0){
    			$html .='</table>';
    	}
    	$html .= '<script type="text/javascript">
						function visualizarSubacao(sbaid)
						{
							var local = "par.php?modulo=principal/subacao&acao=A&sbaid=" + sbaid;
							window.open(local, \'popupSubacao\', "height=650,width=900,scrollbars=yes,top=50,left=200" );
						}

						function visualizarAcao(aciid, indid)
						{
							var local = "par.php?modulo=principal/parAcao&acao=A&aciid=" + aciid + "&indid=" +  indid;
							//window.open(local, \'popupSubacao\', "height=600,width=800,scrollbars=yes,top=50,left=200" );
							window.opener.location.href = local;
						}
					</script>';
    	if($tipoRetorno == 'HTML'){
    		return $htmlPontuado.$html;
    	}else{
    		if( $teste == 1 ){
    			return false;
    		} else {
    			return true;
    		}
    	}
    }else{
    	//vai verificar a distribuição do preenchimento das subações!
    	$html = verificaDistribuicaoSubacoes($itrid);
    	$html .= '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border: none;">
    				<tr>
    					<td colspan="2"  style="background-color:#BFBCB7 " >
    						<b>Não existem mais pendências</b>.
    					</td>
    				</tr>
    			</table>';
    	if($tipoRetorno == 'HTML'){
    		return $html;
    	}else{
    		return true;
    	}
    }
}

function apagaSubacoesQuandoEnvioAnalise($inuid, $itrid = null){
	global $db;

	// CARREGA TODAS AS AÇÕES E SUBAÇÕES DA ENTIDADE
	$itrid = $itrid ? $itrid : $_SESSION['par']['itrid'];
	if($_SESSION['par']['inuid'] != 1){ // verifica se o inuid é DF
		$where = " and i.indid not in (213, 214, 220, 267, 268, 269, 270) ";
	}
	if($_SESSION['par']['estuf'] != "DF"){
		$where = " and i.indid not in (213,214,220,267,268,269,270) ";
	}

	$sql = "SELECT
				aciid,
                acidsc,
                acinomeresponsavel,
                crtpontuacao,
                sbadsc,
                sbaid,
                frmid,
                ptsid,
                cronograma,
                quantidade,
                itens,
                beneficiario,
                escolas,
                qtdescolas,
                numeroescolas,
                tipocronograma,
                indid,
                detalhamento,
                SUM(ano2011) AS ano2011,
                SUM(ano2012) AS ano2012,
                SUM(ano2013) AS ano2013,
                SUM(ano2014) AS ano2014,
                contapreid,
                preid,
                sobano,
                predescricao
                                   FROM
                                   (
                                               SELECT           ac.aciid,
                                                                       ac.acidsc,
                                                                       ac.acinomeresponsavel,
                                                                       c.crtpontuacao,
                                                           s.sbadsc,
                                                                       s.sbaid,
                                                                       s.frmid,
                                                                       s.ptsid,
                                                                       COUNT(sd.sbdinicio) as cronograma,
                                                                       COUNT(sbdquantidade) as quantidade,
                                                                       COUNT(sic.icoid) as itens,
                                                                       COUNT(sabid) as beneficiario,
                                                                       0 AS escolas,
                                                                       0 AS qtdescolas,
                                                                       0 AS numeroescolas,
                                                                       'global' as tipocronograma,
                                                                       i.indid,
                                                                       count(sd.sbddetalhamento) as detalhamento,
                                                                       CASE WHEN sd.sbdano = '2011' THEN 1  ELSE 0 END AS ano2011,
                                                                       CASE WHEN sd.sbdano = '2012' THEN 1  ELSE 0 END AS ano2012,
                                                                       CASE WHEN sd.sbdano = '2013' THEN 1  ELSE 0 END AS ano2013,
                                                                       CASE WHEN sd.sbdano = '2014' THEN 1  ELSE 0 END AS ano2014,
                                                                       count(so.preid) as contapreid,
                                                                       so.preid,
                                                                       so.sobano,
                                                                       po.predescricao
                                               FROM par.dimensao                             d
                                               INNER JOIN par.area                 a  ON a.dimid  = d.dimid AND arestatus = 'A'
                                               INNER JOIN par.indicador          i  ON i.areid  = a.areid AND indstatus = 'A'
                                               INNER JOIN par.criterio             c  ON i.indid  = c.indid AND c.crtstatus = 'A'
                                               INNER JOIN par.pontuacao        p  ON p.crtid  = c.crtid AND p.ptostatus = 'A'
                                               INNER JOIN par.acao                ac ON ac.ptoid = p.ptoid AND ac.acistatus = 'A'
                                               INNER JOIN par.subacao                      s  ON s.aciid  = ac.aciid AND s.sbastatus = 'A' AND s.sbastatus = 'A'  AND s.frmid NOT IN ( 14, 15 ) --não trazer as de emenda parlamentar
                                               LEFT JOIN par.subacaoitenscomposicao            sic ON sic.sbaid = s.sbaid
                                               LEFT JOIN par.subacaodetalhe                          sd  ON sd.sbaid  = s.sbaid --AND sbdano = date_part('year', now())
                                               LEFT JOIN par.subacaobeneficiario        sb  ON sb.sbaid  = s.sbaid
                                               LEFT JOIN par.subacaoobra so
                                                           INNER JOIN obras.preobra po ON po.preid = so.preid  AND po.prestatus = 'A'
                                               ON so.sbaid = s.sbaid
                                               WHERE           d.itrid = {$itrid}
                                                                       AND dimstatus = 'A'
                                                                       AND p.inuid = ".$inuid."
                                                                       AND s.sbaextraordinaria IS NULL
                                                                    --    AND s.sbaid in (4593667)
                                                                       AND s.sbacronograma = 1 -- global
                                             						  ".$where."
                                   GROUP BY ac.aciid, ac.acidsc, ac.acinomeresponsavel, c.crtpontuacao, s.sbadsc, s.sbaid, s.frmid, s.ptsid, tipocronograma, i.indid, sd.sbdano, so.preid, so.sobano, po.predescricao
                                   UNION ALL
                                               SELECT           ac.aciid,
                                                                       ac.acidsc,
                                                                       ac.acinomeresponsavel,
                                                                       c.crtpontuacao,
                                                                       s.sbadsc,
                                                                       s.sbaid,
                                                                       s.frmid,
                                                                       s.ptsid,
                                                                       COUNT(sd.sbdinicio) as cronograma,
                                                                      -- escolas.qtdporescola AS quantidade,
                                                                      COUNT(sbdquantidade) as quantidade,
                                                                       COUNT(sic.icoid) as itens,
                                                                       COUNT(sabid) as beneficiario,
                                                                       escolas2.qtdescola AS escolas ,
                                                                       (select COUNT(se2.sesquantidade) FROM par.subacaoescolas se2
                                                                       inner join par.escolas esc2 on esc2.escid = se2.escid
																       inner join entidade.entidade ent2 on ent2.entid = esc2.entid AND ent2.entstatus = 'A' AND (ent2.entescolanova = false or ent2.entescolanova is null) and ent2.tpcid in (1,3)
																       inner join entidade.funcaoentidade f on f.entid = ent2.entid AND f.funid = 3
																	   WHERE se2.sesstatus = 'A' and ent2.entcodent is not null AND
                                                                       se2.sbaid = s.sbaid AND se2.sesstatus = 'A') AS qtdescolas,
                                                                       (select COUNT(se2.sesid) FROM par.subacaoescolas se2
                                                                       inner join par.escolas esc2 on esc2.escid = se2.escid
																       inner join entidade.entidade ent2 on ent2.entid = esc2.entid AND ent2.entstatus = 'A' AND (ent2.entescolanova = false or ent2.entescolanova is null) and ent2.tpcid in (1,3)
																       inner join entidade.funcaoentidade f on f.entid = ent2.entid AND f.funid = 3
																	   WHERE se2.sesstatus = 'A' and ent2.entcodent is not null AND
                                                                       se2.sbaid = s.sbaid AND se2.sesstatus = 'A') AS numeroescolas,
                                                                       'escola' as tipocronograma,
                                                                       i.indid,
                                                                       count(sd.sbddetalhamento) as detalhamento,
                                                                       CASE WHEN sd.sbdano = '2011' THEN 1  ELSE 0 END AS ano2011,
                                                                       CASE WHEN sd.sbdano = '2012' THEN 1  ELSE 0 END AS ano2012,
                                                                       CASE WHEN sd.sbdano = '2013' THEN 1  ELSE 0 END AS ano2013,
                                                                       CASE WHEN sd.sbdano = '2014' THEN 1  ELSE 0 END AS ano2014,
                                                                       count(so.preid) as contapreid,
                                                                       so.preid,
                                                                       so.sobano,
                                                                       po.predescricao

                                               FROM par.dimensao                              d
                                               INNER JOIN par.area                  a  ON a.dimid = d.dimid AND arestatus = 'A'
                                               INNER JOIN par.indicador           i  ON i.areid = a.areid AND indstatus = 'A'
                                               INNER JOIN par.criterio              c  ON i.indid = c.indid AND c.crtstatus = 'A'
                                               INNER JOIN par.pontuacao         p  ON p.crtid = c.crtid AND p.ptostatus = 'A'
                                               INNER JOIN par.acao                 ac ON ac.ptoid = p.ptoid AND ac.acistatus = 'A'
                                               INNER JOIN par.subacao                       s  ON s.aciid = ac.aciid AND s.sbastatus = 'A' AND s.sbastatus = 'A'  AND s.frmid NOT IN ( 14, 15 ) --não trazer as de emenda parlamentar
                                               LEFT JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
                                           /*    LEFT JOIN (
                                                                       SELECT se.sbaid, COUNT(sei.seiqtd) as qtdporescola
                                                                       FROM par.subacaoescolas se
                                                                       inner join par.escolas esc on esc.escid = se.escid
																	   inner join entidade.entidade ent on ent.entid = esc.entid
                                                                       INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = se.sbaid
                                                                       INNER JOIN par.subescolas_subitenscomposicao sei ON sei.sesid = se.sesid AND sic.icoid = sei.icoid
                                                                       WHERE se.sesstatus = 'A' and ent.entcodent is not null  -- and se.sbaid = 2277231
                                                                       AND (ent.entescolanova = false or ent.entescolanova is null) AND ent.entstatus = 'A' and ent.tpcid in (1,3)
                                                                       GROUP BY se.sbaid
                                                           ) escolas on escolas.sbaid = s.sbaid */

                                               LEFT JOIN (
                                                                       SELECT se.sbaid, COUNT(sesquantidade) as qtdescola
																		FROM par.subacaoescolas se
																		inner join par.subacao s on s.sbaid = se.sbaid
																		inner join par.acao a on a.aciid = s.aciid
																		inner join par.pontuacao p on p.ptoid = a.ptoid AND p.inuid = ".$inuid."
																		inner join par.escolas esc on esc.escid = se.escid
																	   inner join entidade.entidade ent on ent.entid = esc.entid
																	   inner join entidade.funcaoentidade f on f.entid = ent.entid AND f.funid = 3
                                                                       WHERE se.sesstatus = 'A' and ent.entcodent is not null --and se.sbaid = 2277231
                                                                       AND (ent.entescolanova = false or ent.entescolanova is null) AND ent.entstatus = 'A' and ent.tpcid in (1,3)
																		GROUP BY se.sbaid

                                                           ) escolas2 on escolas2.sbaid = s.sbaid
                                               LEFT JOIN par.subacaoitenscomposicao                       sic ON sic.sbaid = s.sbaid
                                               LEFT JOIN par.subacaobeneficiario                    sb ON sb.sbaid = s.sbaid
                                               LEFT JOIN par.subacaoobra so
                                                           INNER JOIN obras.preobra po ON po.preid = so.preid  AND po.prestatus = 'A'
                                               ON so.sbaid = s.sbaid

                                               WHERE           d.itrid = {$itrid}
                                                                       AND dimstatus = 'A'
                                                                      AND p.inuid = ".$inuid."
                                                                      AND s.sbaextraordinaria IS NULL
                                                                      AND s.sbaid NOT IN ( SELECT sbaid FROM par.empenhosubacao where eobstatus = 'A' )
                                                                   --    AND s.sbaid in (4593667) -- TIRAR DEPOIS
                                                                       AND s.sbacronograma = 2 -- por escola
                                                                      ".$where."
                                               GROUP BY ac.aciid, ac.acidsc, ac.acinomeresponsavel, c.crtpontuacao, s.sbadsc, s.sbaid, s.frmid, s.ptsid,
                                               tipocronograma, i.indid, sd.sbdano, so.preid, so.sobano, po.predescricao, --qtdporescola,
                                               escolas2.qtdescola
                        ) AS grupo
                                   GROUP BY aciid,
                                               acidsc,
                                               acinomeresponsavel,
                                               crtpontuacao,
                                               sbadsc,
                                               sbaid,
                                               frmid,
                                               ptsid,
                                               cronograma,
                                               quantidade,
                                               itens,
                                               beneficiario,
                                               escolas,
                                               qtdescolas,
                                               numeroescolas,
                                               tipocronograma,
                                               indid,
                                               detalhamento,
                                               preid,
                                               sobano,
                                               predescricao,
                                               contapreid
                                   ORDER BY aciid";

    $dados = $db->carregar($sql);

    if(is_array($dados)){
	    foreach($dados as $chave => $dado){
	    	// SE TODOS OS DADOS DA SUBACAO FOREM VAZIOS EXCLUI AS SUBAÇÕES.
	    	if( ($dado['cronograma']   == 0) &&
	    		($dado['quantidade']   == 0) &&
	    		($dado['itens'] 	   == 0) &&
	    		($dado['beneficiario'] == 0) &&
	    		($dado['contapreid']   == 0) &&
	    		($dado['escolas'] 	   == 0)
	    	){
	    		$arrAcoes[] = $dado['aciid'];
	    		deletaDadosSubacao( $dado['sbaid'], 'ANALISE' );
	    	}
	    }
    }
	return $arrAcoes;
}

function testaMCMV( $preid ){

	global $db;

	$sql = "SELECT
				CASE WHEN premcmv
					THEN 'N'
					ELSE 'S'
				END as teste
			FROM
				obras.preobra
			WHERE
				preid = $preid";

	return $db->pegaUm($sql);
}

function verificaPendenciaDadosUnidadeIntegrantesEmail(){

	global $db;

	$pendenciaDadoUnidade = 0;

	if($_SESSION['par']['itrid'] == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL){

		$muncod = $_SESSION['par']['muncod'];

		$sql = "(SELECT
						distinct ent.entid, ent.entemail, ent.entnumcpfcnpj
					FROM par.entidade ent
					WHERE ent.entstatus = 'A' AND ent.dutid = ".DUTID_PREFEITURA."
					AND ent.muncod = '{$muncod}' LIMIT 1 )
				union all
					(SELECT
						distinct ent.entid, ent.entemail, ent.entnumcpfcnpj
					FROM par.entidade ent
					INNER JOIN par.instrumentounidade iu ON iu.inuid = ent.inuid
					WHERE ent.entstatus = 'A' AND ent.dutid = ".DUTID_PREFEITO."
					AND iu.muncod = '{$muncod}' LIMIT 1 )
				union all
					(SELECT
						distinct ent.entid, ent.entemail, ent.entnumcpfcnpj
					FROM par.entidade ent
					WHERE ent.entstatus = 'A' AND ent.dutid = ".DUTID_SECRETARIA_MUNICIPAL."
					AND ent.muncod = '{$muncod}' LIMIT 1 )
				union all
					(SELECT
						distinct ent.entid, ent.entemail, ent.entnumcpfcnpj
					FROM par.entidade ent
					INNER JOIN par.instrumentounidade iu ON iu.inuid = ent.inuid
					WHERE ent.entstatus = 'A' AND ent.dutid = ".DUTID_DIRIGENTE."
					AND iu.muncod = '{$muncod}' LIMIT 1 )";
		/*
	} else {

		$estuf = $_SESSION['par']['estuf'];

		$sql = "SELECT
					distinct ent.entid, fun.funid, ent.entemail, ent.entnumcpfcnpj
				FROM entidade.entidade ent
					INNER JOIN entidade.endereco 		eed2 ON eed2.entid = ent.entid
					INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = 6 AND fue.fuestatus = 'A'
					INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid
				WHERE (ent.entstatus = 'A' OR ent.entstatus IS NULL)
				and eed2.estuf = '$estuf'
			union all
				SELECT
					distinct ent.entid, fun.funid, ent.entemail, ent.entnumcpfcnpj
				FROM entidade.entidade ent
					INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = 25 AND fue.fuestatus = 'A'
					INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid
					LEFT JOIN entidade.funentassoc 		fea ON fea.fueid = fue.fueid
					LEFT JOIN entidade.entidade         ent2 ON ent2.entid = fea.entid
					LEFT JOIN entidade.endereco         eed2 ON eed2.entid = ent2.entid
					LEFT JOIN entidade.funcaoentidade 	fue2 ON fue2.entid = ent2.entid AND fue2.funid = 6 AND fue2.fuestatus = 'A'
					LEFT JOIN entidade.funcao 			fun2 ON fun2.funid = fue2.funid
				WHERE (ent.entstatus = 'A' OR ent.entstatus IS NULL)
				and eed2.estuf = '$estuf'";
	*/
	}

	$arDados = $db->carregar($sql);
	$arDados = ($arDados) ? $arDados : array();

	$html = '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border: none;">
    				<tr>
    					<td colspan="2" class="TituloTela"  style="background-color:#6F8F20" >Existem Dados da Unidade com Pendência</td>
    				</tr>';

	if( str_to_upper( $arDados[0]['entemail'] ) == str_to_upper( $arDados[2]['entemail'] ) ){
		//pendencia no email da prefeitura/secretaria
		$pendenciaDadoUnidade = 1;
    	$html .='<tr>
    				<td colspan="2" style="background-color:#E9E9E9" >O e-mail da Prefeitura e da Secretaria não pode ser o mesmo.</td>
				 </tr>';
	}

	if( str_to_upper( $arDados[1]['entemail'] ) == str_to_upper( $arDados[3]['entemail'] ) ){
		if( $arDados[1]['entnumcpfcnpj'] != $arDados[3]['entnumcpfcnpj'] ){
			//pendencia no email da prefeito/secretario
			$pendenciaDadoUnidade = 1;
	    	$html .='<tr>
	    				<td colspan="2" style="background-color:#FCFCFC" >O e-mail do Prefeito e do Secretário não pode ser o mesmo.</td>
					 </tr>';
		}
	}

	if( $pendenciaDadoUnidade == 1 ){
		$html .= '</table>';
		return $html;
	}

}

function verificaDistribuicaoSubacoes($itrid = null){
	global $db;

	if( !$itrid ){
		 $itrid = $_SESSION['par']['itrid'];
	}

	$sql = "SELECT
				sum(total) as total,
				sum(ano2011) as ano2011,
				sum(ano2012) as ano2012,
				sum(ano2013) as ano2013,
				sum(ano2014) as ano2014
			FROM
			(SELECT
				count(sd.sbdano) as total,
				0 as ano2011,
				0 as ano2012,
				0 as ano2013,
				0 as ano2014
			FROM
					par.dimensao d
			INNER JOIN par.area       a  ON a.dimid  = d.dimid AND arestatus = 'A'
            INNER JOIN par.indicador  i  ON i.areid  = a.areid AND indstatus = 'A'
            INNER JOIN par.criterio   c  ON i.indid  = c.indid AND c.crtstatus = 'A'
            INNER JOIN par.pontuacao  p  ON p.crtid  = c.crtid AND p.ptostatus = 'A'
			INNER JOIN par.acao 	  ac ON ac.ptoid = p.ptoid AND ac.acistatus = 'A'
			INNER JOIN par.subacao 	  s  ON s.aciid = ac.aciid AND s.sbastatus = 'A'
			INNER JOIN par.subacaodetalhe 	 sd ON sd.sbaid = s.sbaid
			WHERE
				p.ptostatus = 'A' AND
				p.inuid = ".$_SESSION['par']['inuid']." AND
				d.itrid = ".$itrid."

			UNION ALL

			SELECT
				0 as total,
				count(sd.sbdano) as ano2011,
				0 as ano2012,
				0 as ano2013,
				0 as ano2014
			FROM
					par.dimensao d
			INNER JOIN par.area       a  ON a.dimid  = d.dimid AND arestatus = 'A'
            INNER JOIN par.indicador  i  ON i.areid  = a.areid AND indstatus = 'A'
            INNER JOIN par.criterio   c  ON i.indid  = c.indid AND c.crtstatus = 'A'
            INNER JOIN par.pontuacao  p  ON p.crtid  = c.crtid AND p.ptostatus = 'A'
			INNER JOIN par.acao 	  ac ON ac.ptoid = p.ptoid AND ac.acistatus = 'A'
			INNER JOIN par.subacao 	  s  ON s.aciid = ac.aciid AND s.sbastatus = 'A'
			INNER JOIN par.subacaodetalhe 	 sd ON sd.sbaid = s.sbaid
			WHERE
				p.ptostatus = 'A' AND
				p.inuid = ".$_SESSION['par']['inuid']." AND
				d.itrid = ".$itrid." AND
				sd.sbdano = 2011

			UNION ALL

			SELECT
				0 as total,
				0 as ano2011,
				count(sd.sbdano) as ano2012,
				0 as ano2013,
				0 as ano2014
			FROM
					par.dimensao d
			INNER JOIN par.area       a  ON a.dimid  = d.dimid AND arestatus = 'A'
            INNER JOIN par.indicador  i  ON i.areid  = a.areid AND indstatus = 'A'
            INNER JOIN par.criterio   c  ON i.indid  = c.indid AND c.crtstatus = 'A'
            INNER JOIN par.pontuacao  p  ON p.crtid  = c.crtid AND p.ptostatus = 'A'
			INNER JOIN par.acao 	  ac ON ac.ptoid = p.ptoid AND ac.acistatus = 'A'
			INNER JOIN par.subacao 	  s  ON s.aciid = ac.aciid AND s.sbastatus = 'A'
			INNER JOIN par.subacaodetalhe 	 sd ON sd.sbaid = s.sbaid
			WHERE
				p.ptostatus = 'A' AND
				p.inuid = ".$_SESSION['par']['inuid']." AND
				d.itrid = ".$itrid." AND
				sd.sbdano = 2012

			UNION ALL

			SELECT
				0 as total,
				0 as ano2011,
				0 as ano2012,
				count(sd.sbdano) as ano2013,
				0 as ano2014
			FROM
					par.dimensao d
			INNER JOIN par.area       a  ON a.dimid  = d.dimid AND arestatus = 'A'
            INNER JOIN par.indicador  i  ON i.areid  = a.areid AND indstatus = 'A'
            INNER JOIN par.criterio   c  ON i.indid  = c.indid AND c.crtstatus = 'A'
            INNER JOIN par.pontuacao  p  ON p.crtid  = c.crtid AND p.ptostatus = 'A'
			INNER JOIN par.acao 	  ac ON ac.ptoid = p.ptoid AND ac.acistatus = 'A'
			INNER JOIN par.subacao 	  s  ON s.aciid = ac.aciid AND s.sbastatus = 'A'
			INNER JOIN par.subacaodetalhe 	 sd ON sd.sbaid = s.sbaid
			WHERE
				p.ptostatus = 'A' AND
				p.inuid = ".$_SESSION['par']['inuid']." AND
				d.itrid = ".$itrid." AND
				sd.sbdano = 2013

			UNION ALL

			SELECT
				0 as total,
				0 as ano2011,
				0 as ano2012,
				0 as ano2013,
				count(sd.sbdano) as ano2014
			FROM
					par.dimensao d
			INNER JOIN par.area       a  ON a.dimid  = d.dimid AND arestatus = 'A'
            INNER JOIN par.indicador  i  ON i.areid  = a.areid AND indstatus = 'A'
            INNER JOIN par.criterio   c  ON i.indid  = c.indid AND c.crtstatus = 'A'
            INNER JOIN par.pontuacao  p  ON p.crtid  = c.crtid AND p.ptostatus = 'A'
			INNER JOIN par.acao 	  ac ON ac.ptoid = p.ptoid AND ac.acistatus = 'A'
			INNER JOIN par.subacao 	  s  ON s.aciid = ac.aciid AND s.sbastatus = 'A'
			INNER JOIN par.subacaodetalhe 	 sd ON sd.sbaid = s.sbaid
			WHERE
				p.ptostatus = 'A' AND
				p.inuid = ".$_SESSION['par']['inuid']." AND
				d.itrid = ".$itrid." AND
				sd.sbdano = 2014) as foo";

		$dados = $db->pegaLinha( $sql );

		$mensagem = '<table class="tabela" bgcolor="#E9E9E9" cellSpacing="1" cellPadding="3" align="center" style="border: none;">
    				<tr>
    					<td colspan="2"  style="background-color:#E9E9E9 " ><b>';

		$men = 0;

//		if( $dados['ano2012'] != 0 && $dados['ano2013'] != 0 && $dados['ano2014'] != 0 && $dados['total'] != 0 ){
		if( $dados['ano2013'] != 0 && $dados['ano2014'] != 0 && $dados['total'] != 0 ){
	/*		if($dados['ano2012']/$dados['total']*100 < 20){
				$anos[] = "2012";
				$men = 1;
			}
	*/
			if($dados['ano2013']/$dados['total']*100 < 20){
				$anos[] = "2013";
				$men = 1;
			}
			if($dados['ano2014']/$dados['total']*100 < 20){
				$anos[] = "2014";
				$men = 1;
			}
		}

		if( $men == 1 ){
			$ano = implode(", ",$anos);
			$mensagem .= "A quantidade de subações preenchidas para o(s) ano(s) de ".$ano." é menor que 20% do total das subações elaboradas.<br>";
			$mensagem .= "Planeje a distribuição do preenchimento das subações para os 3 anos de execução.";

		}

		$mensagem .= '</b></td>
    				</tr>
    			</table><br>';

		if( $men == 1 ){
			//return $mensagem;
		}
}


/*
 * regra programa EMI - ENSINO MÉDIO INOVADOR
 */

function verificaPreenchimentoEscolasEMI(){
	global $db;

	if(!$_SESSION['par']['adpid']){
		echo '<script type="text/javascript">
	    		alert("Sessão Expirou.\nFavor selecione o programa novamente!");
	    		window.location.href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa";
	    	  </script>';
	    die;
	}


	$total = $db->pegaUm("SELECT count(emiid) FROM par.pfemiescolas WHERE adpid=".$_SESSION['par']['adpid']);


	if($total == 0){
		return 'Favor selecionar pelo menos uma escola da Aba Lista de Escolas';
	}else{
		return true;
		}
}

function verificarQtdObraProinfancia(){
	global $db;
	$sql = "
			SELECT sum(qtd)
			FROM (
				-- 1 ABERTURA DO PAC
				SELECT 	qtd::INTEGER AS qtd
				FROM 	par.cotasproinfancia2014
				WHERE 	muncod = '{$_SESSION['par']['muncod']}'
				UNION ALL
				-- 2 ABERTURA DO PAC -- COTAS PARA OS MUNICÍPIOS QUE TEM DIREITO A NOVA COTA DE 2014
				SELECT 	aopqtdobrapormunicipio::INTEGER AS qtd
				FROM 	par.cotasproinfancia2014selecao2
				WHERE 	muncod = substring( '{$_SESSION['par']['muncod']}', 0, 7)
			) AS FOO
			";

	return $db->pegaUm($sql);
}

function verificarQtdPreObraProinfancia(){
	global $db;
	$sql = "SELECT 	COUNT(preid) as qtd
			FROM 	obras.preobra
			WHERE 	muncod = '{$_SESSION['par']['muncod']}' AND preanoselecao = 2014";

	return $db->pegaUm($sql);
}


function verificaVigenciaVenciada($preid){
	global $db;

	$sql = "SELECT tooid FROM obras.preobra WHERE preid = $preid";
	$tooid = $db->pegaUm($sql);

	// Se a obra for PAC
	if( $tooid == 1 ){
		$sql = "SELECT
					(MIN(pag.pagdatapagamentosiafi) + 720)::date as prazo,
					MIN(pag.pagdatapagamentosiafi) as data_primeiro_pagamento
				FROM
					par.pagamentoobra po
				INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
				WHERE
					po.preid = ".$preid;
	}else{// Sea obra não for PAC
		$sql = "SELECT
					(MIN(pag.pagdatapagamentosiafi) + 720)::date as prazo,
					MIN(pag.pagdatapagamentosiafi) as data_primeiro_pagamento
				FROM
					par.pagamentoobrapar po
				INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
				WHERE
					po.preid = ".$preid;
	}
	$dataPrimeiroPagamento = $db->carregar($sql);

	if($dataPrimeiroPagamento[0]['data_primeiro_pagamento']){
		$sql = "SELECT popdataprazoaprovado::date, arqid FROM obras.preobraprorrogacao WHERE popstatus = 'A' AND popvalidacao = 't' AND preid = ".$preid;
		$prorrogado = $db->pegaLinha($sql);


		if($prorrogado['popdataprazoaprovado']){
			if( ($prorrogado['popdataprazoaprovado']) < (date('Y-m-d')) ){
				return false;
			}
		}else{
			if( ($dataPrimeiroPagamento[0]['prazo']) < (date('Y-m-d')) ){
				return false;
			}
		}

	}

	return true;
}

function carregarTabelaProInfancia($arPreObrasSemCobertura, $obPreObraControle, $hab_prog_proinf, $escrita, $podeProrrogar = null) {
	global $db;

	header('Content-type: text/html; charset=ISO-8859-1');

	$perfil = pegaArrayPerfil($_SESSION['usucpf']);
	?>
	<table class="tabela" style="width:95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align="left" valign="top" width="90%">

				<table class="tabela" style="width:100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<thead>
						<tr bgcolor="#e9e9e9" align="center">
							<td width="80px" align="center"><b>Prioridade</b>&nbsp<img src="../imagens/ajuda.png" onclick="focamensagem()" width="13px" style="cursor:pointer" /></td>
							<td width="55px" align="center"><b>Ação</b></td>
							<td align="left"><b>Nome da obra</b></td>
							<td align="left"><b>Tipo de obra</b></td>
							<?php //if(in_array(PAR_PERFIL_SUPER_USUARIO, $perfil)){ ?>
							<td align="left"><b>Situação</b></td>
							<td align="left"><b>Termo</b></td>
							<td align="left"><b>Empenho processo</b></td>
							<td align="left"><b>Pagamento</b></td>
							<td align="left"><b>Fim da vigência</b></td>
							<td align="left"><b>Prorrogar (dias)</b></td>
							<td align="left"><b>Processo</b></td>
							<?php //}?>
							<!-- td rowspan="2"><?php //wf_desenhaBarraNavegacao( 1 , array( 'monid' => 1 ) ); ?></td -->
						</tr>
					</thead>
					<tbody id="itensComposicaoSemQuadraCobertura">
						<?php $x=0; ?>
						<?php if(count($arPreObrasSemCobertura)): ?>
							<?php foreach($arPreObrasSemCobertura as $preobra): ?>
								<?php
								$docid = prePegarDocid($preobra['preid']);
								$esdid = prePegarEstadoAtual($docid);
								$cor = ($x % 2) ? "#F7F7F7" : "white";

								$seta_cima = '../imagens/seta_cima.gif';
								$styleCima = 'style="cursor:pointer;"';
								if($x == 0){
									$styleCima = '';
									$seta_cima = '../imagens/seta_cimad.gif';
								}

								$seta_baixo = '../imagens/seta_baixo.gif';
								$styleBaixo = 'style="cursor:pointer;"';
								if(($x + 1) == count($arPreObrasSemCobertura)){
									$styleBaixo = '';
									$seta_baixo = '../imagens/seta_baixod.gif';
								}

								?>
								<tr bgcolor="<?php echo $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor ?>';" id="<?php echo $preobra['preid'];?>">
									<td >
										<span><strong><?php echo str_pad($preobra['preprioridade'], 3, "0", STR_PAD_LEFT); ?></strong></span>&nbsp;&nbsp;&nbsp;
										<img class="baixoSemCobertura" src="<?php echo $seta_baixo; ?>" <?php echo $styleBaixo ?> title="Diminuir Prioridade" />&nbsp;
										<img class="cimaSemCobertura" src="<?php echo $seta_cima; ?>" <?php echo $styleCima ?> title="Aumentar Prioridade" />
									</td>
									<td style="padding-left:15px;">
										<?php if($esdid == WF_TIPO_EM_CADASTRAMENTO): ?>
	<!--										<img class="excluir" src="../imagens/excluir.gif" id="<?php echo $preobra['preid'] ?>" style="cursor:pointer;" />&nbsp;&nbsp;-->
											<img src="../imagens/alterar.gif" class="alterar" style="cursor:pointer" id="<?php echo $preobra['preid'] ?>" />
										<?php else: ?>
											<!-- img src="../imagens/excluir_01.gif" />&nbsp;&nbsp; -->
											<img src="../imagens/alterar.gif" class="alterar" style="cursor:pointer" id="<?php echo $preobra['preid'] ?>" />
										<?php endif; ?>
										<?php
										$data_primeiro_pagamento = $db->pegaUm("SELECT
																						MIN(pag.pagdatapagamentosiafi) as data_primeiro_pagamento
																					FROM
																						par.pagamentoobra po
																					INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
																					WHERE
																						po.preid = ".$preobra['preid']);
										// necessita depara stoid = esdid

//										$sql = "SELECT
//													sto.stoid
//												FROM
//													obras.preobra pre
//												INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod
//												LEFT JOIN obr as.ob rainfraestrutura oi
//													INNER JOIN obr as.si tuacaoobra sto ON sto.stoid = oi.stoid
//												ON oi.preid = pre.preid
//												WHERE
//													pre.preid = ".$preobra['preid'];

										$sql = "SELECT
													doc.esdid
												FROM
													obras.preobra pre
												INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod
												LEFT JOIN obras2.obras obr
													INNER JOIN workflow.documento doc ON doc.docid = obr.docid
												ON obr.preid = pre.preid
												WHERE
													pre.preid = {$preobra['preid']}";

										$verificaSituacao = $db->pegaUm( $sql );
										/*$arrEsdid = Array(OBR_ESDID_EM_EXECUCAO, OBR_ESDID_PARALISADA, OBR_ESDID_EM_LICITACAO,
														  OBR_ESDID_EM_REFORMULACAO, OBR_ESDID_AGUARDANDO_REGISTRO_DE_PRECOS,
														  OBR_ESDID_EM_ELABORACAO_DE_PROJETOS);*/
										/*
										 * A pedido do Francisco dia 10/12/2013
										*/
										$arrEsdid = Array(); // Array(OBR_ESDID_CONCLUIDA);

//										if( $podeProrrogar && $data_primeiro_pagamento && ($verificaSituacao == 12 || $verificaSituacao == 99 || $verificaSituacao == 1 || $verificaSituacao == 5 || $verificaSituacao == 9 || $verificaSituacao == 2 ) ):
										$sqlPrazo= "SELECT
														coalesce(MAX(popdataprazoaprovado)::date, (MIN(pag.pagdatapagamentosiafi) + 720)::date ) - now()::date as prazo
													FROM
														par.pagamentoobra po
													INNER JOIN par.pagamento 			pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
													LEFT  JOIN obras.preobraprorrogacao pop ON pop.preid = po.preid AND popstatus = 'A'
													WHERE
														po.preid = {$preobra['preid']}";

										$prazo = $db->pegaUm($sqlPrazo);


										//$venceuVigenciaDoTermo =  verificaVigenciaVenciada($preobra['preid']);
										// Demanda 950 - A pedido do Tiago Randuz
										$venceuVigenciaDoTermo = true;
										if( $podeProrrogar && $data_primeiro_pagamento && !in_array($verificaSituacao, $arrEsdid) && $venceuVigenciaDoTermo ){
											if( $esdid != WF_TIPO_OBRA_AGUARDANDO_PRORROGACAO && $prazo < 91 ){ ?>
											<br/><button type="button" class="prorrogar" id="<?php echo $preobra['preid'] ?>" style="font-size: 9px;">
												<?php echo "Solicitar Prorrogação<br/>da Vigência" ?>
											</button>
										<?php  }
										} else {
											if( in_array($verificaSituacao, array(OBR_ESDID_CONCLUIDA)) && (in_array( PAR_PERFIL_ADM_OBRAS,$perfil ) || in_array( PAR_PERFIL_SUPER_USUARIO,$perfil ) || in_array( PAR_PERFIL_ADMINISTRADOR,$perfil ) ) && $esdid != WF_TIPO_OBRA_AGUARDANDO_PRORROGACAO ){
												$sql = "SELECT
														    sum(po.pobpercentualpag)
														FROM
														    par.pagamentoobra po
															inner join par.pagamento pag on pag.pagid = po.pagid AND pag.pagstatus = 'A'
														WHERE
														    po.preid = {$preobra['preid']}
														    and pag.pagsituacaopagamento not ilike '%CANCELADO%'";
												$percent = $db->pegaUm($sql);

												if( $percent < 100 ){?>
													<br/><button type="button" class="prorrogar" id="<?php echo $preobra['preid'] ?>" style="font-size: 9px;"><?php echo "Solicitar Prorrogação<br/>da Vigência" ?></button><?
												}
											}
										} ?>
									</td>
									<td>
										<a class="alterar" id="<?php echo $preobra['preid'] ?>"><?php echo $preobra['predescricao'] ?></a>
										<?php if($obPreObraControle->verificaFotosObras($preobra['preid'])): ?>
											<a href="javascript:void(0)">
												<img class="anexos" id="foto_<?php echo $preobra['preid'] ?>" align="absmiddle" border="0" src="../imagens/cam_foto.gif" title="Fotos cadastradas" />
											</a>
										<?php endif; ?>
										<?php if($obPreObraControle->verificaAnexoObras($preobra['preid'])): ?>
											<a href="javascript:void(0)">
												<img class="anexos" id="documento_<?php echo $preobra['preid'] ?>" align="absmiddle" border="0" src="../imagens/clipe.gif" title="Documentos anexos" />
											</a>
										<?php endif; ?>
									</td>
									<td>
										<?php echo $preobra['ptodescricao'] ?>
									</td>
									<?php //if(in_array(PAR_PERFIL_SUPER_USUARIO, $perfil)){?>
									<td>
										<?php
										/* Retirado a pedido do Tiago Radunz no dia 22/07/2014 */
// 									  	if( $preobra['esdid'] == WF_TIPO_EM_CADASTRAMENTO ||
// 									  		$preobra['esdid'] == WF_TIPO_OBRA_APROVADA ||
// 									  		$preobra['esdid'] == WF_TIPO_OBRA_ARQUIVADA ||
// 									  		$preobra['esdid'] == WF_TIPO_OBRA_AGUARDANDO_PRORROGACAO ||
// 									  		$preobra['esdid'] == WF_TIPO_EM_CORRECAO ||
// 									  		$preobra['esdid'] == WF_TIPO_EM_REFORMULACAO){
// 									  		echo $preobra['esddsc'];
// 									  		if( $preobra['esdid'] == WF_TIPO_EM_CORRECAO ){
// 										  		$docid = prePegarDocid($preobra['preid']);

// 												$sql = "SELECT
// 															max(hd.htddata)
// 														FROM workflow.historicodocumento hd
// 														WHERE
// 															docid = {$docid}
// 															AND aedid = 516";

// 												$data = $db->pegaUm($sql);

// 												$arData = explode(" ", $data);

// 												$dataFinal = calculaPrazoDiligencia( $preobra['preid'] );
// 												$arDataInicial = explode("-",$arData[0]);
// 												$dataInicial = $arDataInicial[2]."/".$arDataInicial[1]."/".$arDataInicial[0];
// 												$arDataFinal = explode("-",$dataFinal);
// 												$dataFinal = $arDataFinal[2]."/".$arDataFinal[1]."/".$arDataFinal[0];

// 										  		$sql = "SELECT dioqtddia, diodata FROM par.diligenciaobra WHERE dioativo = 't' AND preid = ".$preobra['preid'];
// 												$dados = $db->pegaLinha( $sql );

// 												if(is_array($dados) && $dados['diodata'] && $dados['dioqtddia']) {
// 													$arData = explode('-', $dados['diodata']);

// 													$ano = $arData[0];
// 													$mes = $arData[1];
// 													$dia = $arData[2];
// 													$dataFinal = mktime(24*$dados['dioqtddia'], 0, 0, $mes, $dia, $ano);
// 													$dataFormatada = date('d/m/Y',$dataFinal);
// 													$dataInicial = $dia . "/" . $mes . "/" . $ano;
// 													$dataFinal = $dataFormatada;
// 												}

// 												echo "<label style=\"color: red\"> (Inicio: ".$dataInicial.". Prazo Final: ".$dataFinal.")";
// 												//echo "<label style=\"color: red\"> (Inicio: ".$dataInicial.". Prazo Final: 31/12/2011)";
// 									  		}
// 									  	}else{
// 									  		echo 'Em Análise';
// 									  	}
                                        echo $preobra['estado1']
									  	?>
									</td>
									<?php //}?>
								<td>
								<?
								/* Trecho que preenche a coluna termo */
								$sql = "SELECT terassinado, to_char(terdatainclusao,'dd/mm/YYYY') as terdatainclusao, tcp.arqid FROM par.termoobra teo
										INNER JOIN par.termocompromissopac tcp ON teo.terid = tcp.terid
										WHERE teo.preid = '".$preobra['preid']."' AND terstatus = 'A'";

								$termoobra = $db->pegaLinha($sql);

								if($termoobra) {
									if($termoobra['terassinado']=="f") {
										echo "Gerado (".($termoobra['terdatainclusao']).")";
									} elseif($termoobra['terassinado']=="t") {
										echo "Assinado";
									}
									if($termoobra['arqid']) {
										echo " <a href=\"par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A&download=S&arqid=".$termoobra['arqid']."\" ><img align=absmiddle src=../imagens/anexo.gif border=0></a>";
									}
								} else {
									echo "Não gerado";
								}

								/* FIM - Trecho que preenche a coluna termo */
								?>
								</td>
								<td>
								<?
								/* Trecho que preenche a coluna empenho */
								$sql = "SELECT emp.empnumero, vve.vrlempenhocancelado as empvalorempenho FROM par.empenhoobra emo
											INNER JOIN par.empenho emp ON emo.empid = emp.empid and eobstatus = 'A' and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
											inner join par.v_vrlempenhocancelado vve on vve.empid = emp.empid
										WHERE emo.preid = '".$preobra['preid']."'";

								$empenhoobra = $db->pegaLinha($sql);

								if($empenhoobra) {
									if($empenhoobra['empnumero']) {
										echo "Gerado (R$".number_format($empenhoobra['empvalorempenho'],2,",",".")." - ".$empenhoobra['empnumero'].")";
									} else {
										echo "Não gerado";
									}
								} else {
									echo "Não gerado";
								}
								/* FIM - Trecho que preenche a coluna empenho */
								?>
								</td>
								<td>
								<?
								/* Substituido dia 23/12/2013 parnumseqob ->   pagnumeroob
								 * Analista: Daniel Areas Programador Eduardo Dunice
								* */
								/* Trecho que preenche a coluna pagamento */
								$sql = "SELECT pro.proagencia, pro.probanco, pag.pagvalorparcela, pag.pagnumeroob, to_char(pag.pagdatapagamento,'dd/mm/YYYY') as pagdatapagamento
										FROM par.empenhoobra emo
										INNER JOIN par.empenho emp ON emp.empid = emo.empid and eobstatus = 'A' and empstatus = 'A'
										INNER JOIN par.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
										INNER JOIN par.pagamento pag ON pag.empid = emo.empid
										WHERE emo.preid = '".$preobra['preid']."' AND pag.pagstatus='A'";

								$pagamentoobra = $db->pegaLinha($sql);

								if($pagamentoobra) {
									echo "Pago<br>
										  Valor pagamento(R$): ".number_format($pagamentoobra['pagvalorparcela'],2,",",".")."<br>
										  Nº da Ordem Bancária: ".$pagamentoobra['pagnumeroob']."<br>
										  Data do pagamento: ".$pagamentoobra['pagdatapagamento']."<br>
										  Banco: ".$pagamentoobra['probanco'].", Agência: ".$pagamentoobra['proagencia']."
										  ";
								} else {
									echo "Não pago";
								}
								/* FIM - Trecho que preenche a coluna pagamento */
								?>
								</td>
								<td>
									<?php
										$sql = "select
													MIN(pag.pagdatapagamentosiafi) as data_primeiro_pagamento,
													MIN(pag.pagdatapagamentosiafi) + 720 as prazo
												from
													par.pagamentoobra po
												inner join par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
												where
													po.preid = ".$preobra['preid'];

										$dataPrimeiroPagamento = $db->carregar($sql);
										if($dataPrimeiroPagamento[0]['data_primeiro_pagamento']){
											$sql = "SELECT popdataprazoaprovado, arqid FROM obras.preobraprorrogacao WHERE popstatus = 'A' AND popvalidacao = 't' AND preid = ".$preobra['preid'];
											$prorrogado = $db->pegaLinha($sql);
											if( $prorrogado['popdataprazoaprovado'] ){
												echo formata_data($prorrogado['popdataprazoaprovado']);
												if( $prorrogado['arqid'] ){ ?>
													<img src="../imagens/consultar.gif" class="consultaprorrogacao" style="cursor:pointer" id="<?php echo $prorrogado['arqid'] ?>" />
												<?php }
											} else {
												echo formata_data($dataPrimeiroPagamento[0]['prazo']);
											}
										}
									?>
								</td>
								<td><?=$prazo; ?></td>
								<td>
									<?php

										$sql = "select distinct
													(
													    CASE WHEN (
														REPLACE( REPLACE( REPLACE(po.ppasaldoconta, ',', ''), '.', ''), '-', '0')::FLOAT +
														REPLACE( REPLACE( REPLACE(po.ppasaldofundos, ',', ''), '.', ''), '-', '0')::FLOAT +
														REPLACE( REPLACE( REPLACE(po.ppasaldopoupanca, ',', ''), '.', ''), '-', '0')::FLOAT +
														REPLACE( REPLACE( REPLACE(po.ppasaldordbcdb, ',', ''), '.', ''), '-', '0')::FLOAT
													    ) > 0 THEN
														'<span class=\"processoDetalhes processo_detalhe\" >'
													    ELSE
														'<span class=\"processo_detalhe\" >'
													    END
													) ||
													to_char(poc.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') || '</span>' as pronumeroprocesso
												from
													par.processoobraspaccomposicao ppc
												inner join par.processoobra poc on poc.proid = ppc.proid and poc.prostatus = 'A'
												inner join obras2.pagamentopac po on poc.pronumeroprocesso = po.ppaprocesso
												where
													ppc.pocstatus = 'A' and
													preid = ".$preobra['preid'];


										$processo = $db->pegaUm($sql);

										echo $processo?$processo:'<center>--</center>';
									?>
								</td>
								</tr>
								<?php $x++ ?>
							<?php endforeach; ?>
						<?php endif; ?>
					</tbody>
					<tfoot>
						<tr>
							<td colspan="15" class="SubTituloEsquerda" style="padding-left:15px;">
								<?php //if($boData){ ?>
								<?php
									if($_REQUEST['programa']){
										if($_SESSION['proinfancia2014']){
											$cotaproinfancia = verificarQtdObraProinfancia();
											$qtdtdpreobra = verificarQtdPreObraProinfancia();
											if($cotaproinfancia > $qtdtdpreobra){
												$hab_prog_proinf = true;
											}
										}
										//ver($cotaproinfancia >= $qtdtdpreobra);
										if($hab_prog_proinf){ ?>
								<div>
                                    <?php
                                    // Se tiver pendências de obras, exibe aviso e impede a inclusão de obras proinfância
                                    $possuiPendenciaObras = $_SESSION['par']['itrid'] == 2 ? getObrasPendentesPAR($_SESSION['par']['muncod']) : getObrasPendentesPAR(null, $_SESSION['par']['estuf']);
                                    if(empty($possuiPendenciaObras)){ ?>
  <!--                                      <span class="<?= $escrita ? 'adicionar' : ''; ?>" style="cursor:pointer; " >
										    Insira suas obras do Proinfância aqui: &nbsp;<img src="../imagens/gif_inclui<?= $escrita ? '' : '_d'; ?>.gif" border="0" align="absmiddle" />
 									    </span> -->
                                    <?php } else {  ?>
                                        <span style="cursor:pointer; " >
										    <a href="#" onclick="javascript: alert('Você não pode executar essa operação pois existe a necessidade de atualização dos dados no sistema de monitoramento de obras.');">Insira suas obras do Proinfância aqui: &nbsp;<img class="middle link" src="../imagens/gif_inclui.gif"/></a>
									    </span>
                                    <?php } ?>
								</div>
								<?php 	}elseif($_SESSION['proinfancia2014'] && ($qtdtdpreobra <  $cotaproinfancia) ){

								?>
									<div>Prazo para o envio de projetos do Proinfância 2014 expirado.</div>
								<?php
										} else if ($qtdtdpreobra >=  $cotaproinfancia) { ?>
									<div>O município atintiu a quantidade máxima de projetos disponíveis.</div>
								<?php 	}
									} ?>
							</td>
						</tr>
						<tr class="SubTituloEsquerda">
							<td colspan="15"><b>Total de Obras:</b>&nbsp;&nbsp;<?php echo count($arPreObrasSemCobertura); ?></td>
						</tr>
					</tfoot>
				</table>
			</td>
		</tr>
	</table>
<?php }

function monta_tabela_execucao( $tipo ){

	global $db;

	if( !$db->testa_superuser() ){
		return false;
	}

	switch ($tipo){
		case TIPO_PAR:
			// empenhopar
			$sql = "(SELECT TO_CHAR(em.empdata,'YYYY') as ano,COALESCE(SUM(vve.vrlempenhocancelado),0.00) as valor, 'EMP' as tipo
					FROM par.empenho em
					INNER JOIN par.processopar pro ON em.empnumeroprocesso = pro.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and pro.prpstatus = 'A' and empstatus = 'A'
					inner join par.v_vrlempenhocancelado vve on vve.empid = em.empid
					WHERE TRIM(em.empsituacao) = '2 - EFETIVADO'
					GROUP BY ano
					ORDER BY ano)
					UNION ALL
					(SELECT to_char(pg.pagdatapagamento,'YYYY') as ano,coalesce(sum(pg.pagvalorparcela),0.00) as valor, 'PAG' as tipo
					FROM par.pagamento pg
					INNER JOIN par.empenho 		em  ON em.empid = pg.empid and empstatus = 'A' and pagstatus = 'A'
					INNER JOIN par.processopar 	pro ON em.empnumeroprocesso = pro.prpnumeroprocesso and pro.prpstatus = 'A'
					WHERE trim(pg.pagsituacaopagamento) = '2 - EFETIVADO'
					GROUP BY ano
					ORDER BY ano)";
			break;
		case TIPO_OBRAS_PAR:
			$sql = "(SELECT TO_CHAR(em.empdata,'YYYY') as ano,COALESCE(SUM(vve.vrlempenhocancelado),0.00) as valor, 'EMP' as tipo
					FROM par.empenho em
					INNER JOIN par.processoobraspar pro ON em.empnumeroprocesso = pro.pronumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and pro.prostatus = 'A' and empstatus = 'A'
					inner join par.v_vrlempenhocancelado vve on vve.empid = em.empid
					WHERE TRIM(em.empsituacao) = '2 - EFETIVADO'
					GROUP BY ano
					ORDER BY ano)
					UNION ALL
					(SELECT TO_CHAR(pg.pagdatapagamento,'YYYY') as ano,COALESCE(SUM(pg.pagvalorparcela),0.00) as valor, 'PAG' as tipo
					FROM par.pagamento pg
					INNER JOIN par.empenho 				em  ON em.empid = pg.empid and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A' and pagstatus = 'A'
					INNER JOIN par.processoobraspar 	pro ON em.empnumeroprocesso = pro.pronumeroprocesso and pro.prostatus = 'A'
					WHERE TRIM(pg.pagsituacaopagamento) = '2 - EFETIVADO'
					GROUP BY ano
					ORDER BY ano)";
			break;
		case TIPO_OBRAS_PAC:
			$sql = "(SELECT TO_CHAR(em.empdata,'YYYY') as ano,COALESCE(SUM(vve.vrlempenhocancelado),0.00) as valor, 'EMP' as tipo
					FROM par.empenho em
					INNER JOIN par.processoobra pro ON em.empnumeroprocesso = pro.pronumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04')  and pro.prostatus = 'A' and empstatus = 'A'
					inner join par.v_vrlempenhocancelado vve on vve.empid = em.empid
					WHERE TRIM(em.empsituacao) = '2 - EFETIVADO'
					GROUP BY ano
					ORDER BY ano)
					UNION ALL
					(SELECT TO_CHAR(pg.pagdatapagamento,'YYYY') as ano, COALESCE(SUM(pg.pagvalorparcela),0.00) as valor, 'PAG' as tipo
					FROM par.pagamento pg
					INNER JOIN par.empenho 		em  ON em.empid = pg.empid and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A' and pagstatus = 'A'
					INNER JOIN par.processoobra pro ON em.empnumeroprocesso = pro.pronumeroprocesso and pro.prostatus = 'A'
					WHERE TRIM(pg.pagsituacaopagamento) = '2 - EFETIVADO'
					GROUP BY ano
					ORDER BY ano)";
			break;
	}

	$arrDados = $db->carregar($sql);

	$arrExec = Array();
	if( is_array($arrDados) ){
		foreach($arrDados as $dados){
			$arrExec[$dados['ano']][$dados['tipo']] = $dados['valor'];
		}
		$coluna_anos 	= '';
		$coluna_emp 	= '';
		$coluna_pag 	= '';
		$total_emp		= 0;
		$total_pag		= 0;
		foreach($arrExec as $ano => $tipos){
			$coluna_anos 	.= "<td><b>$ano</b></td>";
			$coluna_emp 	.= "<td bgcolor=white >R$ ".formata_valor($tipos['EMP'],2)."</td>";
			$coluna_pag 	.= "<td bgcolor=#eeeeee >R$ ".formata_valor($tipos['PAG'],2)."</td>";
			$total_emp		+= $tipos['EMP'];
			$total_pag		+= $tipos['PAG'];
		}
		$coluna_emp 	.= "<td bgcolor=white >R$ ".formata_valor($total_emp,2)."</td>";
		$coluna_pag 	.= "<td bgcolor=#eeeeee >R$ ".formata_valor($total_pag,2)."</td>";
	}
	$html = "<table class=tabela style=width:100% bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center >
				<tr bgcolor=#e9e9e9 align=center >
					<td colspan=".(count($arrExec)+2)." style=font-size:15px; ><b>Execução</b></td>
				</tr>
				<tr><td></td>$coluna_anos<td><b>Total</b></td></tr>
				<tr><td bgcolor=white ><b>Empenhado</b></td>$coluna_emp</tr>
				<tr><td bgcolor=#eeeeee ><b>Pago</b></td>$coluna_pag</tr>
			</table>";

	return $html;
}


/**
 * Método para visualizar o detalhamento de tramits para o status "Em diligência"
 * @author Renato Rodrigues de Araujo <renatorodrigues@mec.gov.br>
 */
function visualizarDetalheDiligencia()
{
    global $db;

    $retornoHtml = "<table>";

    if($_POST['docid']) {
        $sqlBuscaComentariosDiligencia = "
    	    SELECT
            	TO_CHAR(cmddata, 'DD/MM/YYYY') AS data,
            	cmddsc AS comentario
            FROM
            	workflow.comentariodocumento cmd
            	INNER JOIN workflow.historicodocumento hst ON hst.hstid = cmd.hstid
            	INNER JOIN workflow.acaoestadodoc aed ON hst.aedid = aed.aedid
            	INNER JOIN workflow.documento doc ON doc.docid = cmd.docid
            WHERE
            	cmd.docid = " . $_POST['docid'] . "
            	AND (aed.esdiddestino = " . WF_PAR_EM_DILIGENCIA . " OR aed.esdiddestino = " . WF_TIPO_EM_CORRECAO . ")
            ORDER BY
            	cmd.cmddata;
	    ";

        $comentariosDiligencia = $db->carregar($sqlBuscaComentariosDiligencia);

        foreach($comentariosDiligencia as $diligencia) {
            $retornoHtml .= "
                    <tr>
                        <td width=\"90%\">" . $diligencia['data'] . " - " . $diligencia['comentario'] . "</td>
                    </tr>
		    ";
        }
    }

    echo $retornoHtml . "</table>";
    exit;
}

/**
 * Monta a lista de municipios onde existam termos de acordo com os filtros
 * do formulario de pesquisa
 *
 * @global type $db
 * @param array $listaFiltro Filtros do formulario de pesquisa
 * @return VOID
 */
function listarMunicipioTermo(array $listaFiltro)
{
    global $db;

    # Termo de Compromisso do PAC = 99999
    if($listaFiltro['tpdcod'] == 99999) {
        $imagemMais = "
            '<img
                class=link
                id=\"img_mais_' || tp.muncod || '\"
                src=\"/imagens/mais.gif\"
                onclick=\"carregarListaTermo(this.id,\'' || tp.muncod || '\');\"
                border=\"0\"
            />'
        ";

    	# Municipio
        if($listaFiltro['muncod']) {
            $municipio = " AND m.muncod = '{$listaFiltro['muncod']}' ";
        }

        # Filtro por lista de Uf OK
        if(!empty($listaFiltro['estuf'][0])) {
            $listaUf = " AND e.estuf IN( '".join("', '", $listaFiltro['estuf'])."' ) ";
        }

        # Filtro por lista de Municipio OK
        if($listaFiltro['listaMunicipio'][0]) {
            $listaMunicipio = " AND m.muncod IN( '".join("', '", $listaFiltro['listaMunicipio'])."' ) ";
        }

    	$sql = "
            SELECT DISTINCT
                    $imagemMais AS acao,
                    '' AS caixa_todos,
                    e.estdescricao,
                    m.muncod,
                    m.mundescricao
                    || '</td></tr>
                        <tr id=\"listaTermo_' || m.muncod || '\" style=\"display: none;\" >
                            <td id=\"trV_' || m.muncod || '\" colspan=6 ></td>
                    </tr>'
            FROM
                par.termocompromissopac tp
                INNER JOIN par.processoobra p ON p.proid = tp.proid and p.prostatus = 'A'
                JOIN territorios.municipio m ON tp.muncod = m.muncod
                JOIN territorios.estado e ON m.estuf = e.estuf
            WHERE tp.terstatus = 'A'
            {$municipio}
            {$listaMunicipio}
            {$listaUf}
    	";
    } else {
    	$consultaTermo = montarConsultaTermo($listaFiltro);

        # Botao + da lista de municipio
    	$imagemMais = "
	        '<img
	            class=link
	            id=\"img_mais_' || m.muncod || '\"
	            src=\"/imagens/mais.gif\"
	            onclick=\"carregarListaTermo(this.id,\'' || m.muncod || '\');\"
	            border=\"0\"
	        />'
	    ";

	    $colunas = "
	        $imagemMais AS acao,
	        '' AS caixa_todos,
	        e.estdescricao,
	        m.muncod,
	        m.mundescricao
	        || '</td></tr>
	            <tr id=\"listaTermo_' || m.muncod || '\" style=\"display: none;\" >
	                <td id=\"trV_' || m.muncod || '\" colspan=6 ></td>
	        </tr>'
	    ";

	    $sql = str_replace('[COLUNAS]', $colunas, $consultaTermo);
	    $sql = str_replace('[ORDER BY]', 'ORDER BY e.estdescricao ASC', $sql);
//            ver($sql, d);
    }
    $listaMunicipio = new MontaListaAjax($db, true);
    $listaMunicipio->montaLista(
        $sql,
        array('', 'Ação', 'Estado', 'Código', 'Município'),
        10,
        10,
        'N',
        '',
        '',
        '',
        '',
        '',
        '',
        '');
}

/**
 * Monta a lista de TERMOS para cada municipio selecionado e ainda de acordo com
 * os filtros do formulario de pesquisa
 *
 * @global type $db
 * @param array $listaFiltro Filtros do formulario de pesquisa
 * @param string $retorno "SQL" Pode retornar apenas o SQL da consulta ao inves de montar a tabela
 * @return VOID/string $sql
 */
function listarTermo(array $listaFiltro, $retorno = NULL, $booBotao = true)
{

    #Variavel do banco
    global $db;
    # Retira o filtro de multiplos municipios para listar apenas do municipio selecionado
    unset($listaFiltro['listaMunicipio']);
    $consultaTermo = montarConsultaTermo($listaFiltro);

    # Volta selecionado quando for pra edicao
    if($listaFiltro['checked'] === TRUE){
        $attrChecked = " checked=\"checked\" ";
    }

    if ($booBotao) {
        #Icones Obras PAC
        $acaoObrasPac = "
            '<input {$attrChecked} class=\"link listaTermo\" id=\"listaTermo_' || p.proid || '\" type=\"checkbox\" name=\"listaTermo[]\" value=\"'|| p.proid || '\" />
            <input type=\"hidden\" value=\"OBRASPAC\" name=\"tipoTermo[' || p.proid || ']\" />
            <img src=../imagens/icone_lupa.png title=\"Visualizar\" style=cursor:pointer; onclick=consultarTermo('||tp.terid||');>'
        ";

        #Icones Obras PAR
        $acaoObrasPar = "
            '<input {$attrChecked} class=\"link listaTermo\" id=\"listaTermo_' || po.proid || '\" type=\"checkbox\" name=\"listaTermo[]\" value=\"'|| po.proid || '\" />
            <input type=\"hidden\" value=\"OBRASPAR\" name=\"tipoTermo[' || po.proid || ']\" />
            <img src=../imagens/icone_lupa.png title=\"Visualizar\" style=cursor:pointer; onclick=consultarTermoPorDopid('||dp.dopid||');>'
        ";

        #Icones Subacoes PAR
        $acaoPar = "
            '<input {$attrChecked} class=\"link listaTermo\" id=\"listaTermo_' || pp.prpid || '\" type=\"checkbox\" name=\"listaTermo[]\" value=\"'|| pp.prpid || '\" />
            <input type=\"hidden\" value=\"PAR\" name=\"tipoTermo[' || pp.prpid || ']\" />
            <img src=\"../imagens/icone_lupa.png\" style=\"cursor:pointer;\" onclick=consultarTermoPorDopid('||dp.dopid||');>'
        ";
    }

    #Adiciona Campos adicionais na consulta (desde de que eles existam em todas as querys)
    if(!empty($listaFiltro['camposRestricao'])) {
    	$camposRestricao = $listaFiltro['camposRestricao'];
    }

    #Filtro de Visao de Municipio AND res.resstatus = 'A' AND res.resinformar = 'S'
    if(!empty($listaFiltro['viewMunicipio'])) {
    	$viewMunicipio = "  AND res.resstatus = 'A' AND res.resinformar = 'S' ";
    }

    #Filtro de Municipio
    if(!empty($listaFiltro['muncod'])) {
    	$muncodObrasPac = " AND tp.muncod = '" . $listaFiltro['muncod'] ."' ";
    	$muncodPar = " AND mun.muncod = '" . $listaFiltro['muncod'] ."' ";
    }

    #Filtro de UF
    if(!empty($listaFiltro['estuf'][0])) {
    	$estufObrasPac = " AND tp.estuf IN ('" . join(', ', $listaFiltro['estuf']) ."') ";
    	$estufPar = " AND e.estuf IN ('" . join(', ', $listaFiltro['estuf']) ."') ";
    }

     # Filtros Tipo Documento e Tipo de Termo OK
    if($listaFiltro['tpdcod'] != 99999 && !empty($listaFiltro['tpdcod'])) {
    	$tpDocumento = " AND md.tpdcod = {$listaFiltro['tpdcod']} ";
        if (!empty($listaFiltro['mdoid'])) {
            $tpTermo = " AND md.mdoid = {$listaFiltro['mdoid']} ";
        }
    }

    # Filtro por resid
    if(!empty($listaFiltro['resid']) && $listaFiltro['resid'] != 'undefined') {
    	$resid = " AND res.resid = '" . $listaFiltro['resid'] . "' ";
    }

    # Filtro por docid
    if(!empty($listaFiltro['docid']) && $listaFiltro['docid'] != 'undefined') {
    	$docid = " AND doc.docid = '" . $listaFiltro['docid'] . "' ";
    }

    # Filtros Tipo Documento e Tipo de Termo OK
    if($listaFiltro['tpdcod'] != 99999 && !empty($listaFiltro['tpdcod'])) {
    	$tpDocumento = " AND md.tpdcod = {$listaFiltro['tpdcod']} ";
        if (!empty($listaFiltro['mdoid'])) {
            $tpTermo = " AND md.mdoid = {$listaFiltro['mdoid']} ";
        }
    }

    $selectObrasPac = "
    	SELECT DISTINCT";
            if ($booBotao) {$selectObrasPac .="$acaoObrasPac AS acao,"; }
            if ($listaFiltro['botaoacao']) {$selectObrasPac .="{$listaFiltro['botaoacao']} AS acao,"; }
    $selectObrasPac .="
            $camposRestricao
            tp.terid AS numero_do_termo,
            'Termo Compromisso PAC' AS tipo_termo,
            TO_CHAR(
            (SELECT max(fim::date) FROM (
                SELECT
                    CASE WHEN popdataprazoaprovado IS NULL THEN
                        CASE WHEN MIN(pag.pagdatapagamentosiafi) IS NULL
                            THEN MIN(pagdatapagamento) + 720
                            ELSE MIN(pag.pagdatapagamentosiafi) + 720
                        END
                        ELSE popdataprazoaprovado
                    END AS fim
                FROM
                    par.pagamentoobra po
                    INNER JOIN par.pagamento pag ON pag.pagid = po.pagid and pagstatus = 'A'
                    INNER JOIN par.processoobraspaccomposicao ppc ON ppc.preid = po.preid  and ppc.pocstatus = 'A'
                    LEFT JOIN obras.preobraprorrogacao prei ON prei.preid = po.preid AND popstatus = 'A'
                WHERE
                    pag.pagsituacaopagamento = '2 - EFETIVADO'
                    AND ppc.proid = p.proid
                GROUP BY
                    popdataprazoaprovado
            ) AS foo)
            , 'MM/YYYY') AS vigencia,
            probanco AS banco,
            proagencia AS agencia,
            nu_conta_corrente AS conta_corrente,
            (
                SELECT
                     SUM(pagvalorparcela) as valor_pago
                FROM
                     par.pagamento  p
                     INNER JOIN par.empenho e on e.empid = p.empid and empstatus = 'A' and pagstatus = 'A'
                     INNER JOIN par.processoobra pp ON pp.pronumeroprocesso = e.empnumeroprocesso and pp.prostatus = 'A'
                     INNER JOIN par.termocompromissopac d ON d.proid = pp.proid
                 WHERE
                     e.empsituacao = '2 - EFETIVADO'
                     AND pagstatus = 'A'
                     AND terid = tp.terid
                     AND pagsituacaopagamento = '2 - EFETIVADO'
            ) AS valor_pago,
            pronumeroprocesso AS processo,
            'Obras PAC' AS tipo_do_processo
        FROM
            par.termocompromissopac tp
            INNER JOIN par.processoobra p ON p.proid = tp.proid and p.prostatus = 'A'
            LEFT JOIN par.restricaoentidade re ON(tp.proid = re.proidpac)
            LEFT JOIN par.restricaofnde res ON(re.resid = res.resid)
            LEFT JOIN workflow.documento doc ON doc.docid = re.docid
            LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
            WHERE tp.terstatus = 'A'
            {$listaFiltro['whereParametrizado']}
            {$muncodObrasPac}
            {$estufObrasPac}
            {$resid}
            {$viewMunicipio}
            {$docid}
    ";

    $colunas = "";
            if ($booBotao) {$colunas .="$acaoObrasPar AS acao,"; }
            if ($listaFiltro['botaoacao']) {$colunas .="{$listaFiltro['botaoacao']} AS acao,"; }
    $colunas .="
        $camposRestricao
        dp.dopnumerodocumento as numero_do_termo,
        md.mdonome AS tipo_termo,
        dp.dopdatafimvigencia AS vigencia,
        po.probanco AS banco,
        po.proagencia AS agencia,
        po.proseqconta AS conta_corrente,
        (
            SELECT
                sum(pagvalorparcela) as valor_pago
            FROM
                par.pagamento p
                INNER JOIN par.empenho e on e.empid = p.empid and empstatus = 'A' and pagstatus = 'A'
                INNER JOIN par.processoobraspar pp ON pp.pronumeroprocesso = e.empnumeroprocesso and pp.prostatus = 'A'
                INNER JOIN par.documentopar d ON d.proid = pp.proid
            WHERE
                e.empsituacao = '2 - EFETIVADO'
                AND p.pagstatus = 'A'
                AND d.dopid = dp.dopid
                AND p.pagsituacaopagamento = '2 - EFETIVADO'
            GROUP BY d.dopid

        ) AS valor_pago,
        po.pronumeroprocesso AS processo,
        'Obras PAR' AS tipo_do_processo
    ";

    $sqlObrasPar = str_replace('[COLUNAS]', $colunas, $consultaTermo);
    $union = " UNION ";

    #Subacoes do PAR
    $selectPar = "
        SELECT";
            if ($booBotao) {$selectPar .="$acaoPar AS acao,"; }
            if ($listaFiltro['botaoacao']) {$selectPar .="{$listaFiltro['botaoacao']} AS acao,"; }
    $selectPar .="
            $camposRestricao
            dp.dopnumerodocumento as numero_do_termo,
            md.mdonome AS tipo_termo,
            dp.dopdatafimvigencia AS vigencia,
            pp.prpbanco AS banco,
            pp.prpagencia AS agencia,
            pp.prpseqconta AS conta_corrente,
            (
                 SELECT
                    sum(pagvalorparcela) as valor_pago
                 FROM
                    par.pagamento p
                    INNER JOIN par.empenho e on e.empid = p.empid and empstatus = 'A' and pagstatus = 'A'
                    INNER JOIN par.processopar pp ON pp.prpnumeroprocesso = e.empnumeroprocesso and pp.prpstatus = 'A'
                    INNER JOIN par.documentopar d ON d.prpid = pp.prpid
                 WHERE
                    e.empsituacao = '2 - EFETIVADO'
                    AND p.pagstatus = 'A'
                    AND d.dopid = dp.dopid
                    AND p.pagsituacaopagamento = '2 - EFETIVADO'
                 GROUP BY d.dopid
            ) AS valor_pago,
            pp.prpnumeroprocesso AS processo,
            'Subações PAR' as tipo_do_processo
            FROM par.vm_documentopar_ativos dp
            INNER JOIN par.modelosdocumentos md ON md.mdoid = dp.mdoid
            INNER JOIN par.processopar pp ON pp.prpid = dp.prpid
            INNER JOIN par.instrumentounidade iu ON pp.inuid = iu.inuid
            LEFT JOIN territorios.municipio mun ON mun.muncod = iu.muncod
            JOIN territorios.estado e ON mun.estuf = e.estuf
            LEFT JOIN par.restricaoentidade re ON(dp.prpid = re.prpid)
            LEFT JOIN par.restricaofnde res ON(re.resid = res.resid)
            LEFT JOIN workflow.documento doc ON doc.docid = re.docid
            LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
            WHERE 1=1
            {$listaFiltro['whereParametrizado']}
            {$muncodPar}
            {$estufPar}
            {$resid}
            {$viewMunicipio}
            {$docid}
            {$tpDocumento}
            {$tpTermo}
        ";

    if($listaFiltro['tpdcod'] != 99999) {
    	$sqlTodo = $sqlObrasPar . $union . $selectPar;
    } else {
    	$sqlTodo = $selectObrasPac;
    }

    if($retorno == 'SQL'){
        return $selectObrasPac . $union . $sqlObrasPar . $union . $selectPar;
    }
    $listaMunicipio = new MontaListaAjax($db, true);
    $celWidth = array(
    	'10%',
    	'5%',
    	'30%',
    	'10%',
    	'5%',
    	'5%',
    	'10%',
    	'10%',
    	'10%',
    	'15%',
	);

    # cabecalho de termo
    $cabecalho = array(
        'Ação',
        'Termo',
        'Tipo do Termo',
        'Vigência',
        'Banco',
        'Agência',
        'Conta Corrente',
        'Valor Pago(R$)',
        'Processo',
        'Tipo do Processo',
    );
    $listaMunicipio->montaLista(
        $sqlTodo,
        $cabecalho,
        10,
        10,
        'N',
        '',
        '',
        '',
        $celWidth,
        '',
        '',
        '');
}

/**
 * Montar Consulta de termos aplicando os filtros
 *
 * @param array $listaFiltro
 * @return string
 */
function montarConsultaTermo(array $listaFiltro){

    # Municipio
    if($listaFiltro['muncod']) {
        $municipio = " AND m.muncod = '{$listaFiltro['muncod']}' ";
    }

    # Filtros Tipo Documento e Tipo de Termo OK
    if($listaFiltro['tpdcod'] != 99999 && !empty($listaFiltro['tpdcod'])) {
    	$tpDocumento = " AND md.tpdcod = {$listaFiltro['tpdcod']} ";
        if (!empty($listaFiltro['mdoid'])) {
            $tpTermo = " AND md.mdoid = {$listaFiltro['mdoid']} ";
        }
    }

    # Filtro por lista de Uf OK
    if(!empty($listaFiltro['estuf'][0])) {
        $listaUf = " AND m.estuf IN( '".join("', '", $listaFiltro['estuf'])."' ) ";
    }

    # Filtro por lista de Municipio OK
    if($listaFiltro['listaMunicipio'][0]) {
        $listaMunicipio = " AND m.muncod IN( '".join("', '", $listaFiltro['listaMunicipio'])."' ) ";
    }

    # Filtro por resid OK
    if(!empty($listaFiltro['resid']) && $listaFiltro['resid'] != 'undefined' ){
        $resid = " AND res.resid = '{$listaFiltro['resid']}' ";
    }

    #Filtro de Visao de Municipio AND res.resstatus = 'A' AND res.resinformar = 'S'
    if(!empty($listaFiltro['viewMunicipio'])) {
    	$viewMunicipio = "  AND res.resstatus = 'A' AND res.resinformar = 'S' ";
    }

    # Filtro por docid
    if(!empty($listaFiltro['docid']) && $listaFiltro['docid'] != 'undefined') {
    	$docid = " AND doc.docid = '" . $listaFiltro['docid'] . "' ";
    }

    $sql = "
        SELECT DISTINCT
            [COLUNAS]
        FROM
            par.vm_documentopar_ativos dp
            JOIN par.modelosdocumentos md ON md.mdoid = dp.mdoid
            LEFT JOIN par.processoobraspar po ON po.proid = dp.proid and po.prostatus = 'A'
            JOIN par.instrumentounidade iu ON iu.inuid = po.inuid
            JOIN territorios.municipio m ON iu.muncod = m.muncod
            JOIN territorios.estado e ON m.estuf = e.estuf
            LEFT JOIN par.restricaoentidade re ON(dp.proid = re.proidpar)
            LEFT JOIN par.restricaofnde res ON(re.resid = res.resid)
            LEFT JOIN workflow.documento doc ON doc.docid = re.docid
            LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
        WHERE 1=1
        {$listaFiltro['whereParametrizado']}
        {$resid}
        {$listaMunicipio}
        {$listaUf}
        {$tpDocumento}
        {$tpTermo}
        {$municipio}
        {$viewMunicipio}
        {$docid}
    ";
    return $sql;
}

/**
 * Monta a combo de tipo de termo de acordo com o tipo de documento via AJAX
 *
 * @global type $db
 * @return VOID
 */
function carregarListaTipoDeTermo(){
    global $db;
    $tpdcod = !empty($_REQUEST['tpdcod'])?$_REQUEST['tpdcod']:0;
    $sql = "
        SELECT DISTINCT
            mdoid AS codigo,
            mdoid || ' - ' || mdonome AS descricao
        FROM
            par.modelosdocumentos
        WHERE
            mdostatus = 'A'
            AND tpdcod = {$tpdcod}
        ORDER BY
            descricao ASC
    ";

    $db->monta_combo("mdoid", $sql, 'S', 'Selecione...', '', '', 'Tipo de documento', '', '', 'mdoid', '', $mdoid, '', '', ' link ');
    echo '&nbsp; <img src="../imagens/obrig.gif" title="Indica campo obrigatório.">';
}

/**
 * Busca lista de abas da tela de restrições
 *
 * return array
 */
function buscarAbasRestricoes() {

    $listaMenus = array(
        0 => array(
            "id" => 1,
            "descricao" => "Tipo de restrições",
            "link" => "/par/par.php?modulo=sistema/tabelasapoio/restricoes_fnde/tipoRestricao&acao=A"),
        2 => array(
            "id" => 2,
            "descricao" => "Lista de Restrições",
            "link" => "/par/par.php?modulo=sistema/tabelasapoio/restricoes_fnde/restricaoFNDE&acao=A"),
        3 => array(
            "id" => 3,
            "descricao" => "Cadastro/Edição Restrições",
            "link" => "/par/par.php?modulo=sistema/tabelasapoio/restricoes_fnde/cadastrarRestricao&acao=A"),
        4 => array(
            "id" => 4,
            "descricao" => "Acompanhamento de Restrições",
            "link" => "/par/par.php?modulo=sistema/tabelasapoio/restricoes_fnde/acompanhamentoRestricao&acao=A"));

    return $listaMenus;
}

function buscaDadosSolicitanteDemandas($usucpf, $retorno = null)
{
    global $db;

    $sqlSolicitante = "
        SELECT
            usucpf,
            usunome,
            usuemail
        FROM
            seguranca.usuario
        WHERE
            usucpf = '{$usucpf}'
    ";

    $dadosSolicitante = $db->pegaLinha($sqlSolicitante);

    if(!is_null($retorno)) {
        return $dadosSolicitante[$retorno];
    } else {
        return $dadosSolicitante;
    }

}

function enviarEmailNotificacao($dados, $dadosSolicitante, $dmdid = '', $arquivo = '')
{

    global $db;

    $tipo = $db->pegaUm("
        SELECT
        	dmtnome
        FROM
        	par.demandatipo
        WHERE
            dmtid = {$dados['dmtid']}
    ");

    $origem = $db->pegaUm("
        SELECT
        	dmonome
        FROM
        	par.demanda_origem
        WHERE
        	dmoid = {$dados['dmoid']};
    ");

    if($dados['dmdid']) {
        $dmdid = $dados['dmdid'];
    } else if ($dmdid) {
        $dmdid = $dmdid;
    } else {
        $dmdid = '';
    }

    $remetente = array("nome"=>SIGLA_SISTEMA, "email"=>"noreply@mec.gov.br");

    $destinatarios = array($dadosSolicitante['usuemail']);

    $assunto = "[SIMEC-PAR] #{$dmdid} - {$dados['dmdnome']}";

    $mensagem = "
        <p>
            Prezado(a),
        </p>
        <p>
            <b>ID #:</b> {$dmdid}
            <br/><b>Assunto:</b> {$dados['dmdnome']}
            <br/><b>Tipo: </b> {$tipo}
            <br/><b>Origem: </b> {$origem}
            <br/><b>Prioridade: </b> {$dados['dmdprioridade']}
            <br/><b>Urgente: </b> " . (($dados['dmdurgente'] == 'TRUE') ? 'Sim' : 'Não') . "
            <br/><b>Descrição detalhada: </b> {$dados['dmddescricao']}
            <br/><b>Solicitante: </b> " . ucwords(strtolower($dadosSolicitante['usunome'])) . "
            <br/><b>Arquivos: </b>  {$arquivo}
        </p>
        <p>
            Atenciosamente,
            <br/>
            Equipe SIMEC.
        </p>
        <p>
            <small>
                Notificação enviada automaticamente pelo sistema para informar o cadastro de sua solicitação.
                <br/>Por favor, não responda esse e-mail.
            </small>
        </p>
    ";

    return enviar_email($remetente, $destinatarios, $assunto, $mensagem);
}

function form_anexoRetornoDiligencia()
{
    global $db;

    $html = "";
    $html .= "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
    $html .= "<tr>";
    $html .= "<td class=\"SubTituloDireita\" valign=\"top\">Ação</td>";
    $html .= "<td>Retornar para Retorno de Diligência</td>";
    $html .= "</tr>";
    $html .= "<tr>";
    $html .= "<td class=\"SubTituloDireita\" valign=\"top\">Comentário</td>";
    $html .= "<td><textarea style=\"width: 100%; height: 100px;\" name=\"comentariodemanda\" id=\"comentariodemanda\"></textarea></td>";
    $html .= "</tr>";
    $html .= "<tr>";
    $html .= "<td class=\"SubTituloDireita\" valign=\"top\">Anexo</td>";
    $html .= "<td><input type=\"file\" name=\"arquivo\" id=\"arquivo\" /></td>";
    $html .= "</tr>";
    $html .= "</table>";

    $html .= "
        <script>
		    function ajaxFileUpload(){
			    var docid = jQuery(\"input[name='docid']\").val();
                jQuery.ajaxFileUpload({
                    url:'par.php?modulo=principal/demandas/cadDemandas&acao=A&req=anexarArquivo&docid=' + docid,
                    secureuri:false,
                    fileElementId:'arquivo',
                    dataType: 'json',
                    success: function(data,status){
                        if(typeof(data.error) != 'undefined'){
                            if(data.error){
                                //print error
                                alert(data.error);
                            }else{
                                //clear
                                jQuery('#img img').attr('src',url+'cache/'+data.msg);
                            }
                        }
                        jQuery('#arquivo').live('change', function() {
                          ajaxFileUpload();
                        });
                    },
                    error: function(data,status,e){
                        //print error
                        alert(e);
                        jQuery('#arquivo').live('change', function() {
                          ajaxFileUpload();
                        });
                    }
                });
                return false;
            }

		    jQuery('#arquivo').live('change', function() {
                ajaxFileUpload();
            });

        </script>
    ";

    echo $html;

}

function anexoRetornoDiligencia()
{
    global $db;


    // recuperar código da demanda
    $sql = "
        SELECT
	        dmdid
        FROM
            par.demanda
        WHERE
            docid = {$_REQUEST['docid']};
    ";

    $dmdid = $db->pegaUm($sql);

    if($dmdid){

        $campos	= array("dmdid" => $dmdid);

        require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $file = new FilesSimec("demandaanexo", $campos, 'par');

        if($_FILES["arquivo"]['name']!=''){
            $arquivoSalvo = $file->setUpload($_FILES["arquivo"]['name']);
        }

        $retorno = Array('boo' => true, 'msg' => '');
    } else {
        $retorno = Array('boo' => false, 'msg' => 'Operação não pôde ser realizada.');
    }

    $retorno = simec_json_encode($retorno);

    echo $retorno;
}

function verificaPendenciaTermo( $dados ){
	global $db;

	$proid 		= $dados['proid'];
	$processo 	= $dados['processo'];

	$arrDados = carregarPendenciaValorObras( $proid );

	if( (float)$arrDados['vrlempenhado'] > (float)$arrDados['dopvalortermo'] ){
		$html = "<br/><br/><b>O valor do Termo de Compromisso está menor que o valor Empenhado</b><br/>
								Valor do Termo: &nbsp;&nbsp;&nbsp;&nbsp;R$ ".number_format($arrDados['dopvalortermo'],2,',','.')."<br/>
								Valor Empenhado:&nbsp;R$ ".number_format($arrDados['vrlempenhado'],2,',','.');
	}

	if( (float)$arrDados['vlrobras'] > (float)$arrDados['dopvalortermo'] ){
		$html1 = "<br/><br/><b>O somatório dos empenhos das obras está maior que o valor do Termo de Compromisso</b><br/>
								Valor do Termo: &nbsp;&nbsp;R$ ".number_format($arrDados['dopvalortermo'],2,',','.')."<br/>
								Valor das Obras:&nbsp;R$ ".number_format($arrDados['vlrobras'],2,',','.')."";
	}

	$result = "<span style='color: red;'><b>Pendência Obras PAR:</b>".$html.$html1."</span>";

	echo $result;
}

function carregarPendenciaValorObras( $proid ){
	global $db;

	$sql = "SELECT
			    dp.dopid,
			    cast(sum(dp.dopvalortermo) as numeric(20,2)) as dopvalortermo,
			    --sum(emp.empvalorempenho - coalesce(ep.vrlcancelado, 0)) as vrlempenhado,
			    vs.saldo as vrlempenhado,
			    com.vlrobras
			FROM par.processoobraspar pp
			    inner join par.vm_documentopar_ativos dp on dp.proid = pp.proid
			    inner join (
			            select distinct
			                dopid,
			                cast(sum(prevalorobra) as numeric(20,2)) as vlrobras
			            from(
			            select distinct
			                            te.dopid,
			                            o.prevalorobra
			                        from
			                            obras.preobra o
			                            inner join par.termocomposicao te on te.preid = o.preid
			                        where o.prestatus = 'A'
			            ) as foo
			            group by dopid
			        ) com on com.dopid = dp.dopid
			    left join par.vm_saldo_empenho_do_processo vs on vs.processo = pp.pronumeroprocesso and vs.tipo = 'OBRA'
			WHERE pp.proid = $proid
			group by
				dp.dopid,
			    dp.dopvalortermo,
			    com.vlrobras,
				vs.saldo";
	$arrDados = $db->pegaLinha($sql);
	$arrDados = $arrDados ? $arrDados : array();

	return $arrDados;
}

function salvarDadosPagamento( array $arrParam = array() ){
	global $db;

	$mes 			= $arrParam['mes'];
	$ano 			= $arrParam['ano'];
	$pagparcela 	= $arrParam['pagparcela'];
	$valor 			= $arrParam['valor'];
	$empnumero 		= $arrParam['empnumero'];
	$empid 			= $arrParam['empid'];
	$sistema 		= $arrParam['sistema'];
	$anosubacao 	= $arrParam['anosubacao'];
	$obra_sub		= $arrParam['obra_sub'];
	$anosubacao		= $arrParam['anosubacao'];
	$percentual		= $arrParam['percentual'];
	$vlrpagamentoItem = $arrParam['vlrpagamentoItem'];
	$parnumseqob 	= ($arrParam['parnumseqob'] ? "'".$arrParam['parnumseqob']."'" : 'null');

	$sql = "INSERT INTO par.pagamento(pagparcela, pagmes, paganoparcela, pagvalorparcela, paganoexercicio, pagnumeroempenho, empid, usucpf, pagdatapagamento, parnumseqob, pagsituacaopagamento)
			VALUES ({$pagparcela},
					{$mes},
					{$ano},
					{$valor},
					".date('Y').",
					'{$empnumero}',
					{$empid},
					'{$_SESSION['usucpf']}',
					NOW(),
					{$parnumseqob},
					'Enviado ao SIGEF') RETURNING pagid";

	$pagid = $db->pegaUm($sql);

	if( is_array($obra_sub) && !empty($obra_sub[0]) ) {
		if( $sistema == 'PAR' ){
			foreach($obra_sub as $chave=>$sbaid) {
				$pobpercentualpag = str_replace(array(","),array("."),$percentual[$sbaid]);

				$sql = "INSERT INTO par.pagamentosubacao(sbaid, pagid, pobpercentualpag, pobvalorpagamento, pobano)
			    		VALUES ('".$sbaid."', '".$pagid."', '".$pobpercentualpag."', '".str_replace(array(".",","),array("","."),$vlrpagamentoItem[$sbaid])."','".$anosubacao[$chave]."');";
				$db->executar($sql);

				if($pobpercentualpag == '100'){
					$sql = "UPDATE par.subacaodetalhe SET ssuid = 15 WHERE sbaid = $sbaid and sbdano = '{$anosubacao[$chave]}'";
					$db->executar($sql);
				} else {
					$sql = "UPDATE par.subacaodetalhe SET ssuid = 17 WHERE sbaid = $sbaid and sbdano = '{$anosubacao[$chave]}'";
					$db->executar($sql);
				}
				$db->commit();
			}
		}elseif( $sistema == 'OBRA' ){
			foreach($obra_sub as $preid) {

				$percent = 0;
				$percent = str_replace(array(".",","),array("","."),$percentual[$preid]);
				$percent = $percent > 100 ? 100 : $percent;

				$sql = "INSERT INTO par.pagamentoobrapar(preid, pagid, poppercentualpag, popvalorpagamento)
			    		VALUES ('".$preid."', '".$pagid."', '".$percent."', '".str_replace(array(".",","),array("","."),$vlrpagamentoItem[$preid])."') returning popid;";
				$popid = $db->pegaUm($sql);

				$arSldid = $arrParam['sldid'][$preid];
				if( is_array($arSldid) ){
					foreach ($arSldid as $sldid) {
						$sql = "INSERT INTO par.pagamentodesembolsoobras(popid, sldid, pdostatus) values($popid, $sldid, 'A')";
						$db->executar($sql);
					}
				}
				$db->commit();
			}
		}elseif( $sistema == 'PAC' ){
			foreach($obra_sub as $preid) {

				$sql = "INSERT INTO par.pagamentoobra(preid, pagid, pobpercentualpag, pobvalorpagamento)
						VALUES ('".$preid."',
								'".$pagid."',
								'".str_replace(array(","),array("."),$percentual[$preid])."',
								'".str_replace(array(".",","),array("","."), $vlrpagamentoItem[$preid])."'
							) returning pobid";
				$pobid = $db->pegaUm($sql);

				$arSldid = $arrParam['sldid'][$preid];
				if( is_array($arSldid) ){
					foreach ($arSldid as $sldid) {
						$sql = "INSERT INTO par.pagamentodesembolsoobras(pobid, sldid, pdostatus) values($pobid, $sldid, 'A')";
						$db->executar($sql);
					}
				}
				$db->commit();
			}
		}
	}
	$sql = "INSERT INTO par.historicopagamento(pagid, hpgdata, usucpf, hpgparcela, hpgvalorparcela, hpgsituacaopagamento)
			VALUES ({$pagid}, NOW(), '".$_SESSION['usucpf']."', ".$pagparcela.", ".$valor.", 'Enviado ao SIGEF')";
	$db->executar($sql);
	$db->commit();

	return $pagid;
}

function carregaDadosPagamento( $empid ){
	global $db;

	$perfil = pegaPerfilGeral();

	$sql = "SELECT
				'<img align=absmiddle src=../imagens/mais.gif style=cursor:pointer; title=mais onclick=\"carregarHistoricoPagamento('||p.pagid||',this);\">' as mais,
				p.pagid,
				e.empnumeroprocesso,
				p.parnumseqob,
				p.pagparcela,
				p.pagmes,
				p.paganoparcela,
				p.pagvalorparcela,
				u.usunome,
				COALESCE(p.pagsituacaopagamento,'-') as situacao,
				p.paganoexercicio,
				case when p.parnumseqob is null then l.lwsmsgretorno else '-' end as lwsmsgretorno
			FROM
				par.pagamento p
				INNER JOIN par.empenho e ON e.empid = p.empid and empstatus = 'A'
				LEFT JOIN seguranca.usuario u ON u.usucpf = p.usucpf
				left join par.logws l on l.pagid = p.pagid and l.lwsrequestdata in (select max(h1.lwsrequestdata) as data
                                                                                      from par.logws h1
                                                                                      where
                                                                                          h1.pagid = p.pagid)
			WHERE
				p.empid = ".$empid." AND pagstatus='A'
			ORDER BY
				pagparcela";

	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();

	$arrRegistro = array();
	foreach ($arrDados as $key => $v) {
		$boAprovada 	= stristr(trim($v['situacao']), 'SOLICITAÇÃO');
		$boAprovada 	= ( !empty($boAprovada) ? true : false );

		$boAutorizado 	= stristr(trim($v['situacao']), 'AUTORIZADO');
		$boAutorizado 	= ( !empty($boAutorizado) ? true : false );

		$boEnvioSiaf 	= stristr(trim($v['situacao']), 'ENVIADO AO SIAFI');
		$boEnvioSiaf 	= ( !empty($boEnvioSiaf) ? true : false );

		$boSemEnvio 	= stristr(trim($v['situacao']), 'SEM ENVIO');
		$boSemEnvio 	= ( !empty($boSemEnvio) ? true : false );

		$boRejeitado 	= stristr(trim($v['situacao']), 'REJEITADO');
		$boRejeitado 	= ( !empty($boRejeitado) ? true : false );

		$boCancelado 	= stristr(trim($v['situacao']), 'CANCELADO');
		$boCancelado 	= ( !empty($boCancelado) ? true : false );

		$boDevolvido 	= stristr(trim($v['situacao']), 'DEVOLVIDO');
		$boDevolvido 	= ( !empty($boDevolvido) ? true : false );

		$boCanobGerado 	= stristr(trim($v['situacao']), 'CANOB GERADO');
		$boCanobGerado 	= ( !empty($boCanobGerado) ? true : false );

		$boValaGestao 	= stristr(trim($v['situacao']), 'VALA CENTRO DE GESTÃO');
		$boValaGestao 	= ( !empty($boValaGestao) ? true : false );

		$boValaSiaf 	= stristr(trim($v['situacao']), 'VALA SIAFI');
		$boValaSiaf 	= ( !empty($boValaSiaf) ? true : false );

		$boCancelDevol 	= stristr(trim($v['situacao']), 'CANCELADO/DEVOLVIDO');
		$boCancelDevol 	= ( !empty($boCancelDevol) ? true : false );

		$boAgAjReem 	= stristr(trim($v['situacao']), 'AGUARDANDO AJUSTE PARA REEMISSÃO');
		$boAgAjReem 	= ( !empty($boAgAjReem) ? true : false );

		$boEfetivado 	= stristr(trim($v['situacao']), 'EFETIVADO');
		$boEfetivado 	= ( !empty($boEfetivado) ? true : false );

		$boHabilitaCancelamento = true;

		if( $boAprovada ){
			$boHabilitaCancelamento = true;
		}
		if( $boAutorizado || $boEnvioSiaf || $boSemEnvio || $boRejeitado || $boCancelado || $boDevolvido || $boCanobGerado || $boValaGestao || $boValaSiaf || $boCancelDevol || $boAgAjReem){
			$boHabilitaCancelamento = false;
		}
		if( $boEfetivado ){
			$boHabilitaCancelamento = false;
		}

		$acaoHabilCancelar = '<span style="font-size:18px; cursor:pointer;" title="Não é Possível Cancelar o Pagamento" class="glyphicon glyphicon-remove"></span>';

		if(	in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) || in_array(PAR_PERFIL_PAGADOR ,$perfil)){
			if( empty($v['parnumseqob']) ){
				$acaoCancelar = '<span style="font-size:18px; color:red; cursor:pointer;" title="Excluir o Pagamento" class="glyphicon glyphicon-remove" onclick=excluirPagamento('.$v['pagid'].',\''.$v['empnumeroprocesso'].'\');></span>';
				$acaoConsultar = '<span style="font-size:18px; cursor:pointer;" title="Não é Possível Consultar Pagamento" class="glyphicon glyphicon-refresh"></span>';
				$acaoReenviar = '<span style="font-size:18px; color:#228B22; cursor:pointer;" title="Reenviar Pagamento" class="glyphicon glyphicon-share" onclick=reenviarPagamento('.$v['pagid'].',\''.$v['empnumeroprocesso'].'\');></span>';
			} else {
				$acaoConsultar = '<span style="font-size:18px; color:#3276B1; cursor:pointer;" title="Consultar Pagamento" class="glyphicon glyphicon-refresh" onclick=consultarPagamento('.$v['pagid'].',\''.$v['empnumeroprocesso'].'\');></span>';
				$acaoCancelar = '<span style="font-size:18px; color:red; cursor:pointer;" title="Cancelar Pagamento" class="glyphicon glyphicon-remove" onclick=cancelarPagamento('.$v['pagid'].',\''.$v['empnumeroprocesso'].'\');></span>';
			}

			$sqlObrPag = "
			SELECT
				count(s.sbaid) as qtd_sub
			FROM
			par.processopar p
				INNER JOIN par.processoparcomposicao ppc on ppc.prpid = p.prpid
				INNER JOIN par.subacaodetalhe sd on sd.sbdid = ppc.sbdid
				INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
				inner join  par.pagamentosubacao pg ON pg.sbaid = s.sbaid
			WHERE s.ptsid in (69,51,8,39,74,56,9,40)
				and p.prpnumeroprocesso = '{$v['empnumeroprocesso']}'
				and pagid = {$v['pagid']}
			";

			$totalMobPag = $db->pegaUm($sqlObrPag);

			if( $totalMobPag > 0)
			{
				$acaoPagMobiliario = '<span style="font-size:18px; cursor:pointer;" title="Vinculação de Mobiliário e equipamento nas obras" onclick=verMobiliarioObras('.$v['pagid'].'); class="glyphicon glyphicon-home"></span>';
			}
			else
			{
				$acaoPagMobiliario = "";
			}

			$acaoVerobras = '<span style="font-size:18px;  cursor:pointer;" title="Lista de Obras" class="glyphicon glyphicon-list-alt" onclick=verObrasPagamento('.$v['pagid'].');></span>';
		}else{
			$acaoConsultar = "";
			$acaoCancelar = "";
			$acaoVerobras = "";
			$acaoReenviar = "";
			$acaoPagMobiliario = "";
		}

		array_push($arrRegistro, array(
									'mais' 				=> $v['mais'],
									'acao' 				=> $acaoVerobras.' '.$acaoConsultar.' '.($boHabilitaCancelamento ? $acaoCancelar : $acaoHabilCancelar).' '.$acaoReenviar . $acaoPagMobiliario ,
									'parcela' 			=> $v['pagparcela'],
									'pagmes' 			=> $v['pagmes'],
									'paganoparcela'		=> $v['paganoparcela'],
									'pagvalorparcela' 	=> $v['pagvalorparcela'],
									'usunome' 			=> $v['usunome'],
									'situacao' 			=> $v['situacao'],
									'paganoexercicio' 	=> $v['paganoexercicio'],
									'lwsmsgretorno' 	=> $v['lwsmsgretorno']
								)
			);
	}

	echo "<table align=center border=0 class=tabela cellpadding=3 cellspacing=1 width=100%>";
	echo '<tr align="center">
			<td bgcolor="#eeeeee"><span style="font-size:18px;" class="glyphicon glyphicon-list-alt"></span> =<b> Lista de Obras</b> &nbsp; &nbsp;</td>
			<td bgcolor="#eeeeee"><span style="color:#3276B1; font-size:18px" title="Consultar Pagamento" class="glyphicon glyphicon-refresh"></span> = <b>Consultar Pagamento</b> &nbsp; &nbsp;</td>
			<td bgcolor="#eeeeee"><span style="color:red; font-size:18px" title="Cancelar Pagamento" class="glyphicon glyphicon-remove"></span> = <b>Cancelar Pagamento</b> &nbsp; &nbsp;</td>
			<td bgcolor="#eeeeee"><span style="font-size:18px; color:#228B22;" class="glyphicon glyphicon-share"></span> =<b> Reenviar Pagamento</b> &nbsp; &nbsp;</td>
		</tr>';
	echo "<tr>
			<td class=SubTituloCentro colspan=6>Dados de pagamentos</td></tr>
		<tr>
			<td colspan=6>";

	//$cabecalho = array("&nbsp;","&nbsp;","Parcela","Mês","Ano","Valor(R$)","Usuário criação","Situação pagamento","Exercício", "Log Retorno");
	//$db->monta_lista_simples(array(),$cabecalho,500,5,'N','100%',$par2);

	$html = '
			<table class="listagem" width="100%" align="center" cellspacing="0" cellpadding="2" border="0" style="color:333333;">
			<thead>';
	if( $arrRegistro ){
				$html .= '
				<tr>
					<td width="10%" valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" colspan="2"><b>Ação</b></td>
					<td width="05%" valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>Parcela</b></td>
					<td width="05%" valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>Mês</b></td>
					<td width="05%" valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>Ano</b></td>
					<td width="10%" valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>Valor(R$)</b></td>
					<td width="15%" valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>Usuário criação</b></td>
					<td width="15%" valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>Situação pagamento</b></td>
					<td width="05%" valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>Exercício</b></td>
					<td width="30%" valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>Log Retorno/Erro SIGEF</b></td>
				</tr>
			</thead>
			<tbody>';
			$totPago = 0;
			foreach ($arrRegistro as $key => $v){
				$key % 2 ? $cor = "#dedfde" : $cor = "";
				$totPago += (float)$v['pagvalorparcela'];
				$html .= '
				<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
					<td valign="middle" title="Usuário atualização">'.$v['mais'].'</td>
					<td valign="middle" title="Usuário atualização">'.$v['acao'].'</td>
					<td valign="middle" align="right" title="Parcela" style="color:#999999;">'.$v['parcela'].'</td>
					<td valign="middle" align="right" title="Parcela" style="color:#999999;">'.$v['pagmes'].'</td>
					<td valign="middle" align="right" title="Parcela" style="color:#999999;">'.$v['paganoparcela'].'</td>
					<td valign="middle" align="right" title="Parcela" style="color:#999999;">'.number_format($v['pagvalorparcela'], 2, ',', '.').'</td>
					<td valign="middle" title="Usuário atualização">'.$v['usunome'].'</td>
					<td valign="middle" title="Usuário atualização">'.$v['situacao'].'</td>
					<td valign="middle" align="right" title="Parcela" style="color:#999999;">'.$v['paganoexercicio'].'</td>
					<td valign="middle" title="Usuário atualização">'.$v['lwsmsgretorno'].'</td>
				</tr>';
			}
			$html .= '
			</tbody>
			<tfoot>
				<tr>
					<td align="right" title="&nbsp;"></td>
					<td align="right" title="&nbsp;">Totais:</td>
					<td align="right" title="&nbsp;"></td>
					<td align="right" title="&nbsp;"></td>
					<td align="right" title="&nbsp;"></td>
					<td align="right" title="Valor Total Cancelado(R$)">'.number_format( $totPago, 2, ',', '.' ).'</td>
					<td align="right" title="&nbsp;"></td>
					<td align="right" title="&nbsp;"></td>
					<td align="right" title="&nbsp;"></td>
					<td align="right" title="&nbsp;"></td>
				</tr>
			</tfoot>
			</table>
		</td>
	</tr>
	</table>';
	}else{
		$html .= '
			<table width="100%" align="center" cellspacing="0" cellpadding="2" border="0" class="listagem" style="color:333333;">
			<tbody>
				<tr>
					<td align="center" style="color:#cc0000;">Não foram encontrados Histórico de Pagamento.</td>
				</tr>
			</tbody>
		</table>';
	}
	echo $html;
}

function listaHistoricoPagamento($dados) {

	global $db;

	$sql = "SELECT
				u.usunome,
				to_char(hpgdata, 'dd/mm/YYYY HH24:MI:SS') as data,
				hpgsituacaopagamento,
				hpgparcela,
				hpgvalorparcela
			FROM
				par.historicopagamento h
			LEFT JOIN seguranca.usuario u ON u.usucpf = h.usucpf
			WHERE
				h.pagid = ".$dados['pagid']."
				and h.hpgdata in (select max(h1.hpgdata) as data
                                      from par.historicopagamento h1
                                      where
                                          h1.pagid = h.pagid
                                      group by h1.usucpf, h1.hpgsituacaopagamento)
			ORDER BY
				hpgdata desc";
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();

	$html = '
	<table align="left" border="0"  style="width: 100%" cellpadding="3" cellspacing="1">
		<tr>
			<td width="30px" rowspan="30"><img align=absmiddle src="../imagens/seta_filho.gif"></td>
		</tr>
		<tr>
			<td>
				<table align="left" cellspacing="0" cellpadding="2" border="0" style="color:333333; width: 80%">
				<thead>
					<tr>
						<th colspan="5" style="text-align: center;">Historico de Pagamento</th>
					</tr>
					<tr>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>Usuário atualização</b></td>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>Data</b></td>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>Situação</b></td>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>Parcela</b></td>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>Valor parcela</b></td>
					</tr>
				</thead>
				<tbody>';
				foreach ($arrDados as $key => $v){
					$key % 2 ? $cor = "#dedfde" : $cor = "";
					$html .= '
					<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
						<td valign="middle" title="Usuário atualização">'.$v['usunome'].'</td>
						<td valign="middle" title="Data">'.$v['data'].'</td>
						<td valign="middle" title="Situação">'.$v['hpgsituacaopagamento'].'</td>
						<td valign="middle" align="right" title="Parcela" style="color:#999999;">'.$v['hpgparcela'].'</td>
						<td valign="middle" align="right" title="Valor parcela" style="color:#999999;">'.number_format($v['hpgvalorparcela'], 2, ',', '.').'</td>
					</tr>';
				}
				$html .= '
				</tbody>
			</table>
			</td>
		</tr>
	</table>';
	echo $html;
}

function reenviarPagamento( $request ){
	global $db;

	if( $request['tiposistema'] == 'PAR' ){
		$tabela = 'par.historicowsprocessopar';
		$codigo = 'prpid';
		$sql = "select
					e.empcnpj,
				    e.empnumeroprocesso, e.empprogramafnde,
				    e.empnumerosistema, e.empanooriginal,
				    e.empnumero, e.empcodigonatdespesa,
				    pro.prpnumeroprocesso as processo, pro.prpid as codigo,
				    trim(pro.prpcnpj) as cnpj,
				    pro.prpseqconta as sequencialconta, pro.seq_conta_corrente,
				    p.pagid,
				    p.pagvalorparcela as valorpagamento,
				    'CM' as co_programa_fnde,
				    p.pagparcela,
				    p.paganoexercicio,
				    p.paganoparcela,
				    p.pagmes
				from par.pagamento p
					inner join par.empenho e on e.empid = p.empid
				    inner join par.processopar pro on pro.prpnumeroprocesso = e.empnumeroprocesso
				where
					p.pagid = {$request['pagid']}
				    and p.pagstatus = 'A'
				    and e.empstatus = 'A'
				    and pro.prpstatus = 'A'";
	}else{
		if( $request['tiposistema'] == 'PAC' ){
			$tabela = 'par.historicowsprocessoobra';
			$table = 'par.processoobra';
		} else {
			$tabela = 'par.historicowsprocessoobrapar';
			$table = 'par.processoobraspar';
		}
		$codigo = 'proid';
		$sql = "select
					e.empcnpj,
				    e.empnumeroprocesso, e.empprogramafnde,
				    e.empnumerosistema, e.empanooriginal,
				    e.empnumero, e.empcodigonatdespesa,
				    pro.pronumeroprocesso as processo, pro.proid as codigo,
				    trim(procnpj) as cnpj,
				    pro.proseqconta as sequencialconta, pro.seq_conta_corrente,
				    p.pagid,
				    p.pagvalorparcela as valorpagamento,
				    'CM' as co_programa_fnde,
				    p.pagparcela,
				    p.paganoexercicio,
				    p.paganoparcela,
				    p.pagmes
				from par.pagamento p
					inner join par.empenho e on e.empid = p.empid
				    inner join $table pro on pro.pronumeroprocesso = e.empnumeroprocesso
				where
					p.pagid = {$request['pagid']}
				    and p.pagstatus = 'A'
				    and e.empstatus = 'A'
				    and pro.prostatus = 'A'";
	}

	$arrPagamento = $db->pegaLinha($sql);
	$arrPagamento = $arrPagamento ? $arrPagamento : array();

	$usuario 						= $request['wsusuario'];
	$senha   						= $request['wssenha'];
	$nu_processo 					= $arrPagamento['empnumeroprocesso'];
	$nu_documento_siafi_ne 			= substr($arrPagamento['empnumero'],strpos($arrPagamento['empnumero'], 'NE')+2);
	$nu_cgc_favorecido 				= ($arrPagamento['cnpj'] ? $arrPagamento['cnpj']: $arrPagamento['empcnpj']);
	$nu_seq_conta_corrente_favorec 	= $arrPagamento['seq_conta_corrente'];
	$an_referencia 					= $arrPagamento['paganoexercicio'];
	$sub_tipo_documento 			= "01";
	$nu_sistema 					= $arrPagamento['empnumerosistema'];
	$unidade_gestora 				= "153173";
	$gestao 						= "15253";
	$co_programa_fnde 				= $arrPagamento['co_programa_fnde'];
	$parcela						= $arrPagamento['pagparcela'];
	$dadosNE 						= explode("NE", $arrPagamento['empnumero']);
	$an_exercicio 					= $dadosNE[0];
	$id_pagamento					= $arrPagamento['pagid'];
	$nu_mes 						= sprintf("%02d", $arrPagamento['pagmes']);
	$valor 							= $arrPagamento['valorpagamento'];
	$data_created 					= date("c");

	if( $arrPagamento['empcodigonatdespesa'] == 33404100 || $arrPagamento['empcodigonatdespesa'] == 33304100 ){
		$vl_custeio = $arrPagamento['valorpagamento'];
		$vl_capital = "0";
	} else { // Capital
		$vl_custeio = "0";
		$vl_capital = $arrPagamento['valorpagamento'];
	}

	 $sql = "SELECT distinct l.lwsid FROM par.logws l
			    inner join $tabela h ON l.lwsid = h.lwsid
			WHERE
			    h.{$codigo} = {$arrPagamento['codigo']}
				and h.hwpxmlretorno is null
				and h.hwpdataenvio = (select max(hwpdataenvio) from $tabela where {$codigo} = {$arrPagamento['codigo']})
				and l.lwstiporequest = '05'";
    $request_id = $db->pegaUm($sql);

    if( empty($request_id) ){
		$arrParam = array(
						'lwstiporequest' 	=> '05',
		        		'usucpf' 			=> $_SESSION['usucpf'],
		        		'pagid' 			=> $id_pagamento
		        	);
		$request_id = logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'insert' );
	}

	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
			<request_id>$id_pagamento</request_id>
			<nu_cgc_favorecido>$nu_cgc_favorecido</nu_cgc_favorecido>
			<nu_seq_conta_corrente_favorec>$nu_seq_conta_corrente_favorec</nu_seq_conta_corrente_favorec>
			<nu_processo>$nu_processo</nu_processo>
			<vl_custeio>$vl_custeio</vl_custeio>
			<vl_capital>$vl_capital</vl_capital>
			<an_referencia>$an_referencia</an_referencia>
			<sub_tipo_documento>$sub_tipo_documento</sub_tipo_documento>
			<nu_sistema>$nu_sistema</nu_sistema>
			<unidade_gestora>$unidade_gestora</unidade_gestora>
			<gestao>$gestao</gestao>
			<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
			<detalhamento_pagamento>
				<item>
					<nu_parcela>$parcela</nu_parcela>
					<an_exercicio>$an_exercicio</an_exercicio>
					<vl_parcela>$valor</vl_parcela>
					<an_parcela>$an_referencia</an_parcela>
					<nu_mes>{$nu_mes}</nu_mes>
					<nu_documento_siafi_ne>{$nu_documento_siafi_ne}</nu_documento_siafi_ne>
				</item>
			</detalhamento_pagamento>
		</params>
	</body>
</request>
XML;
	//ver(simec_htmlentities($arqXml),d);
	if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
		$urlWS = 'http://hmg.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
	} else {
		$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
	}

	$arrParam = array(
			'lwsrequestdata' 	=> 'now()',
			'lwsurl' 			=> $urlWS,
			'lwsmetodo' 		=> 'solicitar',
			'lwsid' 			=> $request_id
	);
	logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );

	$arrParam = array(
			$codigo 		=> $arrPagamento['codigo'],
			'lwsid' 		=> $request_id,
			'hwpxmlenvio' 	=> str_replace( "'", '"', $arqXml),
			'hwpdataenvio' 	=> 'now()',
			'usucpf' 		=> $_SESSION['usucpf']
	);
	$hwpid = logWsRequisicao($arrParam, 'hwpid', $tabela, 'insert' );

	try {
		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
				->execute();

		$xmlRetorno = $xml;

		$arrParam = array(
				'hwpid'			=> $hwpid,
				'hwpxmlretorno' => str_replace( "'", '"', $xmlRetorno)
		);
		logWsRequisicao($arrParam, 'hwpid', $tabela, 'alter' );

		$arrParam = array(
				'lwsresponsedata' 	=> 'now()',
				'lwsid'				=> $request_id
		);
		logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );

		$xml = simplexml_load_string( stripslashes($xml));

		$result = (integer) $xml->status->result;

		if(!$result) {
			$arrParam = array(
					'lwserro' 		=> true,
					'lwsmsgretorno' => $xml->status->error->message->text,
					'lwsid' 		=> $request_id
			);
			logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );

			$arrParam = array(
					'hwpid' 		=> $hwpid,
					'hwpwebservice' => 'solicitarPagamento - Erro'
			);
			logWsRequisicao($arrParam, 'hwpid', $tabela, 'alter' );

			$msg = $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->error->message->text)."<br><br>";
			echo mensagemSIGEF('SOLICITAÇÃO DE PAGAMENTO', $msg);
		} else {
			$arrParam = array(
					'lwserro' 		=> false,
					'lwsmsgretorno' => $xml->status->message->text,
					'lwsid' 		=> $request_id
			);
			logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );

			$arrParam = array(
					'hwpid' 		=> $hwpid,
					'hwpwebservice' => 'solicitarPagamento - Sucesso'
			);
			logWsRequisicao($arrParam, 'hwpid', $tabela, 'alter' );

			echo mensagemSIGEF('SOLICITAÇÃO DE PAGAMENTO', $xml->status->message->text);

			$sql = "UPDATE par.pagamento SET
						parnumseqob = ".(($xml->body->nu_registro_ob)?"'".$xml->body->nu_registro_ob."'":"NULL")."
					WHERE pagid = $id_pagamento";
			$db->executar($sql);
			$db->commit();
		}
	} catch (Exception $e){
		$arrParam = array(
				'lwserro' 			=> true,
				'lwsresponsedata' 	=> 'now()',
				'lwsid' 			=> $request_id
		);
		logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Solicitar Pagamento encontra-se temporariamente indisponível.Favor tente mais tarde.".'<br>';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		$arrParam = array(
				'hwpid' 		=> $hwpid,
				'hwpwebservice' => 'solicitarPagamento - Erro',
				'hwpxmlretorno' => str_replace( "'", '"', $xmlRetorno).' - Erro Exception: '.$erroMSG
		);
		logWsRequisicao($arrParam, 'hwpid', $tabela, 'alter' );

		echo mensagemSIGEF('SOLICITAÇÃO DE PAGAMENTO', $erroMSG);
	}
}

function excluirPagamento($dados){
	global $db;

	$pagid 		= $dados['pagid'];
	$processo 	= $dados['processo'];

	/* $sql = "DELETE FROM par.pagamentoobrapar WHERE pagid = $pagid";
	 $db->executar($sql); */

	$sql = "UPDATE par.pagamento SET pagstatus = 'I', pagusucpfexclusao = '".$_SESSION['usucpf']."', pagdataexclusao = now()  WHERE pagid = $pagid";
	$db->executar($sql);

	if($db->commit()){
		$msg = 'Pagamento Excluido com Sucesso!';
	} else {
		$msg = 'Problema ao Excluir o Pagamento!';
	}

	$html = '<div style=" border: 1px solid #B7B7B7; font-size: 11px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
				Excluir Pagamento
				<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >'.$msg.'</div>
			</div>
		 	<br>';
	echo $html;
}

function mensagemSIGEF( $title, $msg ){
	$html = '<div style=" border: 1px solid #B7B7B7; font-size: 11px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
				'.$title.'
				<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >'.$msg.'</div>
			</div>
		 	<br>';
	return $html;
}

function cargaViewEmpenhoObras( $processo ){
	global $db;

	$sql = "DELETE FROM par.vm_saldo_empenho_por_obra WHERE processo = '$processo'";
	$db->executar($sql);

	$sql = " INSERT INTO par.vm_saldo_empenho_por_obra (preid, obrid, esfera, uf, muncod, nomeobra, processo, valorempenho, valorcancelado, valorreforco, saldo, tipo)
			(SELECT es.preid, o.obrid, p.preesfera AS esfera, p.estuf AS uf, p.muncod, p.predescricao AS nomeobra, e.empnumeroprocesso AS processo, sum(es.eobvalorempenho) AS valorempenho, sum(ep.vrlcancelado) AS valorcancelado, sum(er.vlrreforco) AS valorreforco, sum(es.eobvalorempenho + COALESCE(er.vlrreforco, 0::numeric) - COALESCE(ep.vrlcancelado, 0::numeric)) AS saldo, 'PAC' AS tipo
		           FROM par.empenho e
		      JOIN par.empenhoobra es ON es.empid = e.empid AND es.eobstatus = 'A'::bpchar
		   JOIN obras.preobra p ON p.preid = es.preid
		   LEFT JOIN obras2.obras o ON o.preid = p.preid AND o.obrstatus = 'A'::bpchar AND o.obridpai IS NULL
		   LEFT JOIN ( SELECT e.empnumeroprocesso, e.empidpai, es.preid, sum(es.eobvalorempenho) AS vrlcancelado
		    FROM par.empenho e
		  JOIN par.empenhoobra es ON es.empid = e.empid AND es.eobstatus = 'A'::bpchar
		  WHERE (e.empcodigoespecie = ANY (ARRAY['03'::bpchar, '13'::bpchar, '04'::bpchar])) AND e.empstatus = 'A'::bpchar AND e.empsituacao <> 'CANCELADO'::bpchar
		  GROUP BY e.empnumeroprocesso, e.empidpai, es.preid) ep ON ep.empidpai = e.empid AND ep.preid = es.preid
		   LEFT JOIN ( SELECT e.empnumeroprocesso, e.empidpai, es.preid, sum(es.eobvalorempenho) AS vlrreforco
		   FROM par.empenho e
		   JOIN par.empenhoobra es ON es.empid = e.empid AND es.eobstatus = 'A'::bpchar
		  WHERE e.empcodigoespecie = '02'::bpchar AND e.empstatus = 'A'::bpchar AND e.empsituacao <> 'CANCELADO'::bpchar
		  GROUP BY e.empnumeroprocesso, e.empidpai, es.preid) er ON er.empidpai = e.empid AND er.preid = es.preid
		  WHERE e.empsituacao <> 'CANCELADO'::bpchar AND (e.empcodigoespecie <> ALL (ARRAY['03'::bpchar, '13'::bpchar, '02'::bpchar, '04'::bpchar])) AND e.empstatus = 'A'::bpchar
				and e.empnumeroprocesso = '$processo'
		  GROUP BY es.preid, p.preesfera, p.estuf, p.muncod, p.predescricao, e.empnumeroprocesso, o.obrid
		UNION ALL
		   SELECT es.preid, o.obrid, p.preesfera AS esfera, p.estuf AS uf, p.muncod, p.predescricao AS nomeobra, e.empnumeroprocesso AS processo, sum(es.eobvalorempenho) AS valorempenho, sum(ep.vrlcancelado) AS valorcancelado, sum(er.vlrreforco) AS valorreforco, sum(es.eobvalorempenho + COALESCE(er.vlrreforco, 0::numeric) - COALESCE(ep.vrlcancelado, 0::numeric)) AS saldo, 'PAR' AS tipo
		           FROM par.empenho e
		      JOIN par.empenhoobrapar es ON es.empid = e.empid AND es.eobstatus = 'A'::bpchar
		   JOIN obras.preobra p ON p.preid = es.preid
		   LEFT JOIN obras2.obras o ON o.preid = p.preid AND o.obrstatus = 'A'::bpchar AND o.obridpai IS NULL
		   LEFT JOIN ( SELECT e.empnumeroprocesso, e.empidpai, es.preid, sum(es.eobvalorempenho) AS vrlcancelado
		    FROM par.empenho e
		   JOIN par.empenhoobrapar es ON es.empid = e.empid AND es.eobstatus = 'A'::bpchar
		  WHERE (e.empcodigoespecie = ANY (ARRAY['03'::bpchar, '13'::bpchar, '04'::bpchar])) AND e.empstatus = 'A'::bpchar AND e.empsituacao <> 'CANCELADO'::bpchar
		  GROUP BY e.empnumeroprocesso, e.empidpai, es.preid) ep ON ep.empidpai = e.empid AND ep.preid = es.preid
		   LEFT JOIN ( SELECT e.empnumeroprocesso, e.empidpai, es.preid, sum(es.eobvalorempenho) AS vlrreforco
		   FROM par.empenho e
		   JOIN par.empenhoobrapar es ON es.empid = e.empid AND es.eobstatus = 'A'::bpchar
		  WHERE e.empcodigoespecie = '02'::bpchar AND e.empstatus = 'A'::bpchar AND e.empsituacao <> 'CANCELADO'::bpchar
		  GROUP BY e.empnumeroprocesso, e.empidpai, es.preid) er ON er.empidpai = e.empid AND er.preid = es.preid
		  WHERE e.empsituacao <> 'CANCELADO'::bpchar AND (e.empcodigoespecie <> ALL (ARRAY['03'::bpchar, '13'::bpchar, '02'::bpchar, '04'::bpchar])) AND e.empstatus = 'A'::bpchar
				and e.empnumeroprocesso = '$processo'
		  GROUP BY es.preid, p.preesfera, p.estuf, p.muncod, p.predescricao, e.empnumeroprocesso, o.obrid) ";

    $db->executar($sql);
	return $db->commit();
}


/**
 * Funçoes referentes ao Grafico PNE do PAR
 */

function geraGraficoPNE($nomeUnico, $dados)
{
    $meta = $dados['metaBrasil'];
    $metaTotal = $dados['metaTotal'];
    $metaFormatada = number_format($meta, 0, ',', '.');

    $complemento = $simbolo = '';

    $casasDecimais = 0;

    switch ($dados['tipo']) {
        case ('P'):
            $complemento = $simbolo = '%';
            $metaFormatada = $meta;
            $casasDecimais = 1;
            break;
        case ('A'):
            $complemento = ' anos';
            /**
             * Victor Martins Machado
             * Incluído para mostrar o valor com uma casa decimal
             */
            $metaFormatada = $meta;
            $casasDecimais = 1;
            break;
        case ('M'):
            $complemento = ' matrículas';
            break;
        case ('T'):
            $complemento = ' títulos';
            break;
    }

    $formatter = "function () { return '<div style=\"text-align:center\"><span style=\"font-size:20px;color:black; font-weight: normal;\">' + number_format(this.y, " . $casasDecimais .", ',', '.') + '" . $simbolo ."</span><br/>' +
                                       '<span style=\"font-size:13px;color:black; font-weight: normal;\">" . $dados['descricao'] . "</span></div>'; }";

    ?>

    <script type="text/javascript">
        jQuery(function () {

            var gaugeOptions = {

                chart: {
                    type: 'solidgauge'
                },

                title: null,


                credits: {
                    enabled: false
                },
                exporting: {
                    enabled: false
                },

                credits: {
                    enabled: false
                },
                exporting: {
                    enabled: false
                },

                pane: {
                    center: ['50%', '85%'],
                    size: '120%',
                    startAngle: -90,
                    endAngle: 90,
                    background: {
                        backgroundColor: (Highcharts.theme && Highcharts.theme.background2) || '#EEE',
                        innerRadius: '60%',
                        outerRadius: '100%',
                        shape: 'arc'
                    }
                },

                tooltip: {
                    enabled: false
                },

                // the value axis
                yAxis: {
                    stops: [
                        [0, '<?php echo $dados['cor']; ?>']
                    ],
                    lineWidth: 0,
                    minorTickInterval: null,
                    tickPixelInterval: 400,
                    tickWidth: 0,
                    title: {
                        y: -55
                    },
                    labels: {
                        enabled: false
                    },
                    plotBands: [{
                        from: 0,
                        to: parseFloat('<?php echo $meta; ?>'),
                        color: '<?php echo $dados['plotBandsCor'] == '' ? '#726D70' : $dados['plotBandsCor']; ?>',
                        innerRadius: '100%',
                        outerRadius: '<?php echo $dados['plotBandsOuterRadius'] == '' ? '110%' : $dados['plotBandsOuterRadius']; ?>'
                    }]
                },

                plotOptions: {
                    solidgauge: {
                        dataLabels: {
                            y: 5,
                            borderWidth: 0,
                            useHTML: true
                        }
                    }
                }
            };

            // The speed gauge
            jQuery('#<?=$nomeUnico?>').highcharts(Highcharts.merge(gaugeOptions, {
                yAxis: {
                    min: 0,
                    max: parseFloat('<?php echo $metaTotal; ?>'),
                    title: {
                        text: '<span style="color:black;"><?php echo $dados['title'] == '' ? 'Meta Brasil:' : $dados['title']; ?> <?php echo $metaFormatada . $complemento; ?><?php echo $dados['anoprevisto'] != '' ? ' - '.$dados['anoprevisto'] : ''; ?></span>'
                    }
                },

                credits: {
                    enabled: false
                },

                series: [{
                    name: 'Meta',
                    data: [0],
                    dataLabels: {
                        y: 25,
                        formatter: <?php echo $formatter; ?>
                    },
                    tooltip: {
                        valueSuffix: ' '
                    }
                }]

            }));

            // Bring life to the dials
            var chart = $('#<?=$nomeUnico?>').highcharts();
            if (chart) {
                var point = chart.series[0].points[0],
                    newVal = parseFloat('<?php echo $dados['valor']; ?>');
                point.update(newVal);
            }
        });
    </script>
    <div id="<?=$nomeUnico?>" style="width: 200px; height: 150px; float: left"></div>
<?
}

function ativarConselheiroFundeb($arrRegistro) {
	global $db;
	
	if(is_array($arrRegistro) ) {

		foreach($arrRegistro as $key => $v) {

			$existe_usu = $db->pegaUm("select usucpf from seguranca.usuario where usucpf='".str_replace(array(".","-"),array(""),$v['cpf'])."'");
			
			$pos = strpos($v['email'], '@');
			$posuser = strpos($v['usuemail'], '@');
			
			if ($pos === false) {
				if ($posuser === false) $v['email'] = 'null';
				else $v['email'] = $v['usuemail'];
			} else {
				$v['email'] = "'".$v['email']."'";
			}
			 
			if(!$existe_usu) {					 
				$sql = "INSERT INTO seguranca.usuario(usucpf, usunome, usuemail, suscod, usustatus, ususenha, usufoneddd, usufonenum, usufuncao, usudatainc, muncod, regcod)
		     			VALUES ('".str_replace(array(".","-"),array(""),$v['cpf'])."', 
							'".addslashes($v['conselheiro'])."', 
							".$v['email'].", 'A', 'A', 
							'".md5_encrypt_senha("simecdti","")."',
							'".$v['ddd']."',
							'".$v['telefone']."',
							'".$v['cargo']."',
							now(),
							'".$v['muncod']."',
							'".$v['estuf']."'
							);";
				$db->executar($sql);					 
			} else {					 
				$sql = "UPDATE seguranca.usuario SET usustatus='A', suscod='A', 
							usucpf = '".str_replace(array(".","-"),array(""),$v['cpf'])."', 
							usunome = '".addslashes($v['conselheiro'])."', 
							usuemail = ".$v['email'].",
							usufoneddd = '".$v['ddd']."', 
							usufonenum = '".$v['telefone']."', 
							usufuncao = '".$v['cargo']."', 	
							muncod = '".$v['muncod']."', 
							regcod = '".$v['estuf']."',
							suscod = 'A', 
							usustatus = 'A'
						WHERE usucpf='".str_replace(array(".","-"),array(""),$v['cpf'])."'";
				$db->executar($sql);
			}
			 
			$remetente = array("nome" => SIGLA_SISTEMA. " - MÓDULO PAR","email" => $v['email']);
			$destinatario = $v['email'];
			$usunome = $v['conselheiro'];
			$assunto = "Cadastro no SIMEC - MÓDULO PAR";
			$conteudo = "<br/><span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span><br/><span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span><br/><br/>";
			$conteudo .= sprintf("%s %s, <p>Você foi cadastrado no SIMEC, módulo PAR. Sua conta está ativa e, para acessa-la basta entrar no SIMEC (http://simec.mec.gov.br), digitar o seu CPF e senha.</p>
 							  <p>Se for o seu primeiro acesso, o sistema solicitará que você crie uma nova senha. Se você já tiver cadastro no SIMEC, insira o seu CPF e senha. Caso tenha esquecido a sua senha de acesso ao SIMEC, clique em \"Esqueceu a senha?\" e insira o seu CPF. O sistema enviará a sua nova senha para o e-mail que você cadastrou. Em caso de dúvida, entre em contato com a sua Secretaria de Educação.</p>
 							  <p>Sua Senha de acesso é: %s</p>
 							  <br><br>* Caso você já alterou a senha acima, favor desconsiderar este e-mail.",
						'Prezado(a)',
						$v['conselheiro'],
						md5_decrypt_senha($db->pegaUm("SELECT ususenha FROM seguranca.usuario WHERE usucpf='".$v['cpf']."'"),'')
				);

			if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				enviar_email( $remetente, $_SESSION['email_sistema'], $assunto, $conteudo );
			} else {
				if( $v['email'] ){
					enviar_email( $remetente, $destinatario, $assunto, $conteudo );
				}
			}
			 
			$existe_sis = $db->pegaUm("select usucpf from seguranca.usuario_sistema where usucpf='".str_replace(array(".","-"),array(""),$v['cpf'])."' and sisid = 23");

			if(!$existe_sis) {
				$sql = "INSERT INTO seguranca.usuario_sistema(usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod)
		     			VALUES ('".str_replace(array(".","-"),array(""),$v['cpf'])."', 23, 'A', NULL, NOW(), 'A');";					 
				$db->executar($sql);					 
			} else {
				$sql = "UPDATE seguranca.usuario_sistema SET susstatus = 'A', suscod = 'A' WHERE usucpf='".str_replace(array(".","-"),array(""),$v['cpf'])."' AND sisid = 23";
				$db->executar($sql);
			}
				 
			$sql = "INSERT INTO seguranca.historicousuario(htudsc, htudata, usucpf, sisid, suscod, usucpfadm)
    				VALUES ('Mudança realizada pela ferramenta de gerencia do PAR.',
    						NOW(),
    						'".str_replace(array(".","-"),array(""),$v['cpf'])."',
    						'23', 'A', '".$_SESSION['usucpf']."');";				 
			$db->executar($sql);
				 
			$existe_pfl = $db->pegaUm("select usucpf from seguranca.perfilusuario where usucpf='".str_replace(array(".","-"),array(""),$v['cpf'])."' and pflcod = '".$v['pflcod']."'");
			 
			if(!$existe_pfl) {
				$sql = "INSERT INTO seguranca.perfilusuario(usucpf, pflcod) VALUES ('".str_replace(array(".","-"),array(""),$v['cpf'])."', '".$v['pflcod']."');";
				$db->executar($sql);
			}

			$rpuid = $db->pegaUm("select rpuid from par.usuarioresponsabilidade where usucpf='".str_replace(array(".","-"),array(""),$v['cpf'])."' and pflcod='".$v['pflcod']."' AND rpustatus='A'");
			 
				
			$at_entid = "SELECT MAX(ent.entid) FROM entidade.entidade ent
						INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
						WHERE fen.funid=1 AND entnumcpfcnpj IN(SELECT iuecnpj FROM par.instrumentounidadeentidade ie 
				   													INNER JOIN par.instrumentounidade iu ON iu.inuid = ie.inuid WHERE iu.muncod='".$v['muncod']."' AND iu.itrid=2)";
							 
			if(!$rpuid) {
				$sql = "INSERT INTO par.usuarioresponsabilidade(pflcod, usucpf, rpustatus, rpudata_inc, estuf, muncod, entid)
						VALUES ('".$v['pflcod']."', '".str_replace(array(".","-"),array(""),$v['cpf'])."', 'A', NOW(), '".$v['estuf']."', '".$v['muncod']."', ($at_entid))";
				$db->executar($sql);
			} else {
				$sql = "UPDATE par.usuarioresponsabilidade SET estuf='".$v['estuf']."', muncod = '".$v['muncod']."', entid = ($at_entid) WHERE rpuid='".$rpuid."'";
				$db->executar($sql);
			}

			 
			$rpuid = $db->pegaUm("select rpuid from par.usuarioresponsabilidade where usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' and pflcod='".$v['pflcod']."' AND rpustatus='A'");
			 
			$db->commit();		
		}
	}
}

?>
