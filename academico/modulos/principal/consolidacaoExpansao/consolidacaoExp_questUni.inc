<?php

function gravar1( $request ){
	
	global $db;
	
	extract($request);
	
	$sql = "DELETE FROM academico.resposta WHERE entid = ".$_SESSION['academico']['entid']." AND perid = $perid";
	$db->executar($sql);
	
	$sql = "INSERT INTO academico.resposta( perid, entid, resdescricao, resjustificativa, usucpf)
   			VALUES ( $perid, ".$_SESSION['academico']['entid'].", '$resdescricao', '$resjustificativa', '".$_SESSION['usucpf']."');";
	if($db->executar($sql)){
		$db->commit();
		echo "<script>alert('Dados gravados.');window.location = window.location</script>";
	}else{
		$db->rollback();
		echo "<script>alert('Erro ao gravar.');window.location = window.location</script>";
	}
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']($_REQUEST);
	die();
}

require_once APPRAIZ . "includes/classes/entidades.class.inc";

unset($_SESSION['academico']['entidcampus']);

if( $_REQUEST['campus'] ){
	echo "<script>
			window.location = 'academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A';
		  </script>";
	die();
}

$_SESSION['academico']['entid'] = $_REQUEST['entid'] ? $_REQUEST['entid'] : $_SESSION['academico']['entid'];
$entid = $_SESSION['academico']['entid'] ? $_SESSION['academico']['entid'] : $_REQUEST['entid'];
$orgid = $_SESSION['academico']['orgid'];

$existecampus = academico_existeentidade( $entid );

if( !$existecampus ){
	echo "<script>
			alert('A Unidade informada não existe!');
			history.back(-1);
		  </script>";
	die;
}

$docid = pegaDocidConsolidacao();
// instancia o objeto do módulo 
$academico = new academico();

$perfilNotBloq = array(//PERFIL_IFESCADBOLSAS, 
					   PERFIL_CADASTROGERAL, 
					   PERFIL_MECCADBOLSAS,
					   PERFIL_MECCADCURSOS,
					   PERFIL_MECCADASTRO,
					   PERFIL_ADMINISTRADOR,
					   PERFIL_REITOR,
					   PERFIL_ALTA_GESTAO,
					   PERFIL_ASSESSORIA_ALTA_GESTAO);
// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);

$bloqueado = $permissoes['gravar'] ? false :  true;
$bloq = '';
$hide = '';
if($bloqueado) {
	$bloq = 'disabled="disabled"';
	$hide = 'hide';
}
// Busca a unidade pai do Campus
$autoriazacaoconcursos = new autoriazacaoconcursos();
$unidade = ($_REQUEST['entidunidade'] || $_SESSION['academico']['entid']) ? $entid : $autoriazacaoconcursos->buscaentidade($entid);

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

if($unidade) {
	$_SESSION['academico']['entid'] 		= $unidade;
	$_SESSION['academico']['entidadenivel'] = 'unidade';
	$_SESSION['sig_var']['entid'] 			= $_SESSION['academico']['entid'];
	$_SESSION['sig_var']['iscampus'] 		= 'nao';
	
	$edtdsc = $db->pegaUm("SELECT entd.edtdsc FROM academico.entidadedetalhe entd WHERE entd.entid='".$_SESSION['sig_var']['entid']."'");
	$edtdsc = simec_htmlspecialchars(str_replace(array("\r\n","\n\r","\r","\n"), '\n', $edtdsc), ENT_QUOTES);
}

// Testando se a varival $unidade esta correta (feito pelo Alexandre para mapear erros ocorridos)
if(!$unidade) {
	echo "<script>alert('Unidade não selecionada.');window.location='?modulo=inicio&acao=C';</script>";
	exit;
}

$url = 'academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questUni&acao=A';
$sql = "SELECT
			resdescricao
		FROM 
			academico.pergunta per
		LEFT JOIN academico.resposta res ON res.perid = per.perid AND entid = ".$_SESSION['academico']['entid']."
		WHERE
			pernumero = '1.'";
$resp = $db->pegaUm($sql);
if( $resp ){
	$abacod_tela = '57520';
}
$db->cria_aba( $abacod_tela, $url, $parametros );
monta_titulo( "Programa de Consolidação das IFES - Universidade", "");

$orgid = $_SESSION['academico']['orgid'];
$orgao = "Educação Superior";

$sql = "SELECT 
			ent.entid as unidadeorcid, 
			ent.entnome as unidadeorc, 
			ende.estuf, 
			mundescricao,
			entunicod 
		FROM 
			entidade.entidade ent
		LEFT JOIN entidade.endereco 	ende ON ende.entid = ent.entid 
		LEFT JOIN territorios.municipio mun  ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf 
		WHERE 
			ent.entid = '". $entid ."' 
		ORDER BY 
			ent.entnome";
	
$dados = $db->pegaLinha( $sql );

$cabecalho = '';
if($dados) {
	$cabecalho = "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>"
	. "<tr>"
	. "<td class='SubTituloDireita' width='250px;'>Tipo Ensino:</td><td>".$orgao."</td>"
	. "</tr>"
	. "<tr>"
	. "<td class='SubTituloDireita'>Instituição:</td><td>".$dados['unidadeorc']." - ".$dados['entunicod']."</td>"
	. "</tr>"
	. "<tr>"
	. "<td class='SubTituloDireita'>UF / Munícipio:</td><td>".$dados['estuf']." / ".$dados['mundescricao']."</td>"
	. "</tr>"
	. "</table>";
} else {
	$cabecalho .=("<script>
			alert('Foram encontrados problemas nos parâmetros. Caso o erro persista, entre em contato com o suporte técnico');
			window.location='?modulo=inicio&acao=C';
		 </script>");
}
		   
echo $cabecalho;	

$sql = "SELECT 
			per.perid,
			tppid, 
			pernumero, 
			pertitulo, 
			pernivel, 
			perordem, 
			perjutificativa, 
			pertamanho, 
			percondicao,
			resdescricao,
			resjustificativa,
			usucpf
		FROM 
			academico.pergunta per
		LEFT JOIN academico.resposta res ON res.perid = per.perid AND entid = ".$_SESSION['academico']['entid']."
		WHERE
			pernumero = '1.'";

$dados = $db->pegaLinha($sql);
?>
<form name="formulario" method="POST" action="">
	<input type="hidden" name="req" value=""/>
	<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center' >
		<tr>
			<td colspan="2">
				<table cellspacing="0" cellpadding="3" border="0" bgcolor="#f5f5f5" align="center" style="border-top: none;width:60%" class="tabela">
					<tr>
						<td bgcolor="#DCDCDC" align="center"></td>
						<td bgcolor="#DCDCDC" align="center"><b>Metas Pactuadas</b></td>
						<td bgcolor="#DCDCDC" align="center"><b>Metas Realizadas</b></td>
					</tr>
					<?php 
					$bloq = verificaEstadoPreenchimentoEntidade( $_SESSION['academico']['entid'] );
					$bloq = $bloq ? 'S' : 'N';
					$sql = "SELECT
								descr as itmdsc,
								sum(total_previsto) as previsto,
								sum(total_realizado) as realizado
							FROM
							(
							(
							SELECT 
								replace(replace(itm.itmdsc,'(Previsto)','*'),'(Previstos)','*') as descr, 
								itm.itmid, 
								SUM(cpivalor) as total_previsto,
								0 as total_realizado
							FROM 
								entidade.entidade entcampus
							INNER JOIN entidade.funcaoentidade 	 fn ON entcampus.entid = fn.entid AND fn.funid = 18 AND entcampus.entstatus = 'A'
							INNER JOIN entidade.funentassoc 	  a ON a.fueid = fn.fueid
							INNER JOIN entidade.entidade 	entinst ON entinst.entid = a.entid
							INNER JOIN entidade.funcaoentidade 	fn2 ON entinst.entid = fn2.entid AND fn2.funid = 12 AND entinst.entstatus = 'A'
							INNER JOIN academico.campus 		cmp ON cmp.entid = entcampus.entid
							INNER JOIN academico.campusitem 	cpi ON cpi.cmpid = cmp.cmpid
							INNER JOIN academico.item 			itm ON itm.itmid = cpi.itmid
							WHERE 
								entinst.entid = 388679
								AND cpi.cpiano = '2012'
								AND cpi.itmid in (10, 2, 5)
								AND cpi.cpitabnum = 0
							GROUP BY itm.itmdsc, itm.itmid
							ORDER BY itm.itmdsc, itm.itmid)
							UNION ALL
							(
							SELECT 
								replace(itm.itmdsc,'(Realizado)','*') as descr,  
								itm.itmid, 
								0 as total_previsto,
								SUM(cpivalor) as total_realizado
							FROM 
								entidade.entidade entcampus
							INNER JOIN entidade.funcaoentidade 	 fn ON entcampus.entid = fn.entid AND fn.funid = 18 AND entcampus.entstatus = 'A'
							INNER JOIN entidade.funentassoc 	  a ON a.fueid = fn.fueid
							INNER JOIN entidade.entidade 	entinst ON entinst.entid = a.entid
							INNER JOIN entidade.funcaoentidade 	fn2 ON entinst.entid = fn2.entid AND fn2.funid = 12 AND entinst.entstatus = 'A'
							INNER JOIN academico.campus 		cmp ON cmp.entid = entcampus.entid
							INNER JOIN academico.campusitem 	cpi ON cpi.cmpid = cmp.cmpid
							INNER JOIN academico.item 			itm ON itm.itmid = cpi.itmid
							WHERE 
								entinst.entid = ".$_SESSION['academico']['entid']."
								AND cpi.cpiano = '2010'
								AND cpi.itmid in (29, 28, 30)
								AND cpi.cpitabnum = 0
							GROUP BY itm.itmdsc, itm.itmid
							ORDER BY itm.itmdsc, itm.itmid
							)
							) as foo
							GROUP BY itmdsc
							ORDER BY 1";
					$quadros = $db->carregar($sql);
					$quadros = $quadros ? $quadros : Array();
					foreach( $quadros as $dado){
					?>
					<tr>
						<td align="right"><?=str_replace(Array('(Previstos)','(Previsto)'),'*',$dado['itmdsc']) ?></td>
						<td align="right"><?=str_replace('..',',',formata_numero($dado['previsto'])) ?></td>
						<td align="right"><?=str_replace('..',',',formata_numero($dado['realizado'])) ?></td>
					</tr>
					<?php 
					}
					?>
				</table>
				<center>
					*Obs.: As metas realizadas de cursos e vagas foram calculadas com base no SISU nos Editais de 2012.
				</center>
			</td>
		</tr>
		<tr>
			<td colspan="2" style="font-size:12px;" >
				<b><?=$dados['pernumero']?>  <?=$dados['pertitulo'] ?>
				<input type="hidden" name="perid" value="<?=$dados['perid'] ?>"/></b>
			</td>
		</tr>
		<tr>
			<td width="30%"></td>
			<td>
				<input type="radio" name="resdescricao" value="TRUE" <?=($dados['resdescricao'] == 'TRUE' ? 'checked="checked"' : '')  ?> <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>/> Sim
				<input type="radio" name="resdescricao" value="FALSE" <?=($dados['resdescricao'] == 'FALSE' ? 'checked="checked"' : '') ?> <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>/> Não
<!--				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">-->
			</td>
		</tr>
		<tr>
			<td width="30%" align="right"> Justifique:</td>
			<td>
				<?php $resjustificativa = $dados['resjustificativa'];?>
				<?=campo_textarea('resjustificativa', 'N', $bloq, '', 80, 5, $dados['pertamanho']) ?>
			</td>
		</tr>
	</table>
	<table cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" align="center" style="border-top: none; border-bottom: none;" class="tabela">
		<tr>
			<td width="100%" align="center">
				<input type="button" value="Gravar" id="btnGravar" <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>/>
				<? if( $dados['resdescricao'] != '' ){?>
				<input type="button" value="Proximo" id="btnProx"/>
				<? } ?>
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

//jQuery.noConflict();

$(document).ready(function(){
	$('#btnGravar').click(function(){
//		var vazio = true;
//		$('[name="resdescricao"]').each(function(){
//			if( $(this).attr('checked') ){
//				vazio = false;
//			}else{
//				$(this).focus();
//			}
//		});
//		$('.obrigatorio').each(function(){
//			if( $(this).val() == '' ){
//				$(this).focus();
//				vazio = true;
//				return false;
//			}
//		});
//		if( vazio ){
//			alert('Pergunta não respondida.');
//			return false;
//		}
		$('[name="req"]').val('gravar1');
		$('[name="formulario"]').submit();
	});
	$('#btnProx').click(function(){
		window.location = 'academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A';
	});
});

</script>