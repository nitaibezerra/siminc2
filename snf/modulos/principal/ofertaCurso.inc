<?php

if($_GET['curid']){
	$_SESSION['snf']['curid'] = $_GET['curid'];
}
if($_GET['ncuid']){
	$_SESSION['snf']['ncuid'] = $_GET['ncuid'];
}
if($_GET['modid']){
	$_SESSION['snf']['modid'] = $_GET['modid'];
}
if($_REQUEST['requisicaoAjax']){
	$_REQUEST['requisicaoAjax']();
	exit;
}

if (!IS_PRODUCAO) {
	$docid = 109;
} else { 
	$docid = 103;
}

function excluirOferta()
{
	global $db;
	$ofeid = $_POST['ofeid'];
	$sql = "delete from snf.oferta where ofeid = $ofeid";
	$db->executar($sql);
	$db->commit($sql);
	listaOfertas();
}

function addOferta()
{
	global $db;
	extract($_POST);
	$pobid = !$pobid ? "null" : $pobid;
	$polid = !$polid ? "null" : $polid;
	$cmpid = !$cmpid ? "null" : $cmpid;
	$ofeqtde = !$ofeqtde ? "null" : str_replace(".","",$ofeqtde);
	
	$sql = "select popid from snf.projetopedagogico where insid = $insid and curid = $curid and modid = $modid";
	$popid = $db->pegaUm($sql);
	if(!$popid){
		$sql = "insert into snf.projetopedagogico (insid,curid, modid, possuigraduacao, politicaacesso, 
            objetivocurso, cargahoraria, cargahorariapresencial) values 
            ($insid,$curid,$modid,
            ".(($possuigraduacao)?$possuigraduacao:"NULL").",
            ".(($politicaacesso)?$politicaacesso:"NULL").",
            ".(($objetivocurso)?"'".$objetivocurso."'":"NULL").",
            ".(($cargahoraria)?"'".$cargahoraria."'":"NULL").",
            ".(($cargahorariapresencial)?"'".$cargahorariapresencial."'":"NULL")."
            );";
	}else{
		$sql = "update
					snf.projetopedagogico
				set
		            possuigraduacao = ".(($possuigraduacao)?$possuigraduacao:"NULL").",
		            politicaacesso = ".(($politicaacesso)?$politicaacesso:"NULL").",
		            objetivocurso = ".(($objetivocurso)?"'".$objetivocurso."'":"NULL").",
		            cargahoraria = ".(($cargahoraria)?"'".$cargahoraria."'":"NULL").",
		            cargahorariapresencial = ".(($cargahorariapresencial)?"'".$cargahorariapresencial."'":"NULL")."
					
				where
					popid = $popid";
	}
	$db->executar($sql);
	
	if(!$ofeid){
		$sql = "insert into snf.oferta (curid,modid,pobid,polid,cmpid,ofeqtde,ofeano,insid) values ($curid,$modid,$pobid,$polid,$cmpid,$ofeqtde,$ofeano,$insid);";
	}else{
		$sql = "update
					snf.oferta
				set
					pobid = $pobid,
					polid = $polid,
					cmpid = $cmpid,
					insid = $insid,
					ofeqtde = $ofeqtde,
					ofeano = '$ofeano'
				where
					ofeid = $ofeid";
	}
	
	$db->executar($sql);
	$db->commit();
	listaOfertas();
}

function listaOfertas()
{
	
	
	global $db;
	$sql = "	select '<img style=\"cursor:pointer\" src=\"../imagens/alterar.gif\" onclick=\"alterarOferta(' || o.ofeid || ',\'2\',' || pu.pobid || ',\'' || ofeqtde || '\')\" /> <img onclick=\"excluirOferta(' || o.ofeid || ')\" style=\"cursor:pointer\" src=\"../imagens/excluir.gif\" />' as acao,en.estuf, m.mundescricao, 'Pólo UAB', pu.pobdescricao, o.ofeqtde, o.ofeano
				from snf.oferta o
	            inner join snf.polouab pu
	            using(pobid)
	            inner join snf.instituicaouab iu
	            on iu.pobid = pu.pobid
	            inner join snf.instituicaoensino ies
	            on ies.insid = iu.insid
	            inner join snf.enderecopolo en
	            on en.endid = pu.endid
	            inner join territorios.municipio m
	            on m.muncod = en.muncod
				where o.curid = {$_SESSION['snf']['curid']}
				and ies.insid = {$_SESSION['snf']['insid']}
				UNION
				select '<img style=\"cursor:pointer\" src=\"../imagens/alterar.gif\" onclick=\"alterarOferta(' || o.ofeid || ',\'1\',' || pu.polid || ',\'' || ofeqtde || '\')\" /> <img onclick=\"excluirOferta(' || o.ofeid || ')\" style=\"cursor:pointer\" src=\"../imagens/excluir.gif\" />' as acao,en.estuf, m.mundescricao, 'Pólo', pu.poldescricao, o.ofeqtde, o.ofeano
				from snf.oferta o
	            inner join snf.polo pu
	            using(polid)
	            inner join snf.instituicaoensino ies
	            on ies.insid = pu.insid
	            inner join snf.enderecopolo en
	            on en.endid = pu.endid
	            inner join territorios.municipio m
	            on m.muncod = en.muncod
				where o.curid = {$_SESSION['snf']['curid']}
				and ies.insid = {$_SESSION['snf']['insid']}
				UNION
				select '<img style=\"cursor:pointer\" src=\"../imagens/alterar.gif\" onclick=\"alterarOferta(' || o.ofeid || ',\'3\',' || c.cmpid || ',\'' || ofeqtde || '\')\" /> <img onclick=\"excluirOferta(' || o.ofeid || ')\" style=\"cursor:pointer\" src=\"../imagens/excluir.gif\" />' as acao,en.estuf, m.mundescricao, 'Campus', ent.entnome, o.ofeqtde, o.ofeano
				from snf.oferta o
	            inner join academico.campus c
	            on o.cmpid = c.cmpid 
	            inner join entidade.entidade ent
	            on c.entid = ent.entid
	            inner join entidade.funcaoentidade fen
	            on fen.entid = ent.entid
	            and fen.funid = 18
	            inner join entidade.funentassoc fea
	            on fea.fueid = fen.fueid
	            inner join entidade.entidade I
	            on I.entid = fea.entid
	            inner join entidade.funcaoentidade fies
	            on fies.entid = I.entid
	            and fies.funid in (11,12)
	            inner join snf.instituicaoensino ies
	            on ies.entid = fies.entid
	            inner join entidade.endereco en
	            on c.entid = en.entid
	            inner join territorios.municipio m
	            on m.muncod = en.muncod
				where o.curid = {$_SESSION['snf']['curid']}
				and ies.insid = {$_SESSION['snf']['insid']}";
	$cabecalho = array('Ação','UF','Município','Tipo de Unidade de Oferta','Unidade de Oferta','Número de Vagas','Ano');
	$db->monta_lista($sql, $cabecalho, 100, 10, 'N', '', '', '', '');
}

function selecionaUnidade()
{
	global $db;
	$tipo = $_POST['tipo'];
	$insid = $_POST['insid'];
	
	if(!$insid){
		echo "Você não possui instituição vinculada.<input type='hidden' name='hdn_unidade' id='hdn_unidade' value='1' />";	
	}else{
		switch($tipo){
			case 1:
				$sql = "select polid as codigo, poldescricao as descricao 
						from snf.polo p
						inner join snf.instituicaoensino ies using(insid)
						where ies.insid = $insid";
				$db->monta_combo('polid', $sql, "S", 'Selecione...', 'unidadeSelecionada', '', '', '', 'S','polid');
				echo "<input type='hidden' name='hdn_unidade_selecionada' id='hdn_unidade_selecionada' value='0' /><input style='margin-left:10px' type='button' name='tbn_cadastrar' value='Cadastrar Novo' onclick='cadastrarNovo()' />";
			break;
			case 2:
				$sql = "select pobid as codigo, pobdescricao as descricao 
						from snf.polouab p
						inner join snf.instituicaouab i using(pobid)
						inner join snf.instituicaoensino ies using(insid)
						where ies.insid = $insid";
				$db->monta_combo('pobid', $sql, "S", 'Selecione...', 'unidadeSelecionada', '', '', '', 'S','pobid');
				echo "<input type='hidden' name='hdn_unidade_selecionada' id='hdn_unidade_selecionada' value='0' />";
			break;
			case 3:
				$perfil = pegaPerfilGeral();
				if (!$db->testa_superuser())
				{
				
				$sql = "select c.cmpid as codigo, ent.entnome as descricao 
						from  entidade.entidade ent 
						inner join entidade.funcaoentidade fen  on fen.entid = ent.entid and fen.funid in(18)
						inner join academico.campus c on c.entid = ent.entid
						inner join entidade.funentassoc fea on fea.fueid = fen.fueid 
						inner join entidade.entidade ent2 on ent2.entid = fea.entid 
						inner join entidade.funcaoentidade fen2 on fen2.entid = ent2.entid and fen2.funid in(12,11)
						inner join snf.instituicaoensino ies on ies.entid = ent2.entid
						where ies.insid in (select 
												ins.insid 
											from 
												snf.instituicaoensino ins 
											inner join 
												snf.usuarioresponsabilidade ure ON ure.insid = ins.insid 
											and 
												pflcod = 702 
											and 
												ure.usucpf = '{$_SESSION['usucpf']}') 
						group by c.cmpid, ent.entnome
					order by ent.entnome";
				}
				else
				{
					$sql = "SELECT distinct entid as codigo
					from snf.instituicaoensino where insid = {$insid }";
					$id = $db->pegaUm($sql);
					
					$sql = "select c.cmpid as codigo, ORGAOFUNCAO.entnome as descricao
							 from entidade.funentassoc f
							inner join entidade.funcaoentidade fe
							           using(fueid)
							inner join academico.campus c
							           ON c.entid = fe.entid
							inner join entidade.entidade ENTIDADE
							           ON ENTIDADE.entid = f.entid
							inner join entidade.entidade ORGAOFUNCAO
							           ON ORGAOFUNCAO.entid = fe.entid
							where f.entid = $id --- IES
							 and fe.funid = 18 ---- Funcao Campus";
				}
				$db->monta_combo('cmpid', $sql, "S", 'Selecione...', 'unidadeSelecionada', '', '', '', 'S','cmpid');
				echo "<input type='hidden' name='hdn_unidade_selecionada' id='hdn_unidade_selecionada' value='0' />";
			break;
			default:
				echo "Selecione o tipo de unidade de oferta.<input type='hidden' name='hdn_unidade' id='hdn_unidade' value='1' />";	
		}
	}
}

$mostraWorkflow = 'S';
include_once APPRAIZ . 'includes/workflow.php';
/*
 * Recupera o "docid"
 * Caso não exista "docid" cadastrado na demanda 
 * Insere o documento e vincula na demanda
 */
// $docid = criarDocumento( $dmdid );


//Chamada de programa

if ( verificaPerfil( array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR) ) ){
	$insid 					  = isset($_GET['insid']) ? $_GET['insid'] : $_SESSION['snf']['insid'];
	$_SESSION['snf']['insid'] = $insid;
	
	if ( empty( $insid ) ){	
		//Chamada de programa
		include  APPRAIZ."includes/cabecalho.inc";
		?>
		<br />
		<?php 
		$db->cria_aba($abacod_tela,$url,'');
		monta_titulo('Oferta de Cursos',  obrigatorio()." Indica campos obrigatórios.");
	}
?> 
	<link href="../snf/geral/snf.css" type="text/css" rel="stylesheet"></link>
	<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

	<script>
	function selecionaInstituicao(insid){
		location.href = '?modulo=principal/ofertaCurso&acao=A&curid=<?php echo $_GET['curid'] ?>&modid=<?php echo $_GET['modid'] ?>&ncuid=<?php echo $_GET['ncuid'] ?>&insid=' + insid;
	}
	</script>
	<?php 
	$sql = "SELECT
				DISTINCT
				ie.insid AS codigo,
				ie.insnome AS descricao
			FROM
				snf.dirigentemaximo dir
			JOIN
				snf.instituicaoensino ie ON ie.insid = dir.insid AND
											ie.insstatus = 'A'
			WHERE
				dir.dirstatus = 'A'
			ORDER BY
				descricao";
	
	$htmlComboEstado = '
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td colspan="2" class="subtituloCentro" >Selecione uma Instituição</td>
			</tr>
			<tr class="center SubtituloDireita" >
				<td colspan="2" align="center">' 
				.
				$db->monta_combo("combo_insid", $sql, "S", "Selecione uma Instituição...", "selecionaInstituicao", "", "", '', "N", '', true, $_REQUEST['insid'])
				. '
				</td>
			</tr>
		</table>';
				
	if ( empty($insid) ){
		echo $htmlComboEstado;
		die("<script>jQuery('#aguarde').hide();</script>");
	}
}


include  APPRAIZ."includes/cabecalho.inc";
echo "<br/>";
$boAtivo = 'S';
monta_titulo('Oferta de Cursos',  obrigatorio()." Indica campos obrigatórios.");
if($_POST){
	extract($_POST);
}

?>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<script>
	function selecionaUnidade(tipo)
	{
		var insid = $("#insid").val();
		$.ajax({
		url		: 'snf.php?modulo=principal/ofertaCurso&acao=A',
		type	: 'post',
		data	: 'requisicaoAjax=selecionaUnidade&tipo='+tipo+'&insid='+insid,
		success	: function(res){
				$('#td_unidade').html(res);
				if($('#hdn_value_unidade').val()){
					$('#td_unidade select').val($('#hdn_value_unidade').val());
					$("[name='hdn_unidade_selecionada']").val("1");
					$('#hdn_value_unidade').val("");
				}
			}
		});	
	}
	
	function addOferta()
	{
		var erro = 0;

		if(!$("[name='objetivocurso']").val()){
			alert('Objetivo do Curso.');
			erro = 1;
			return false;
		}
		if(!$("[name='cargahoraria']").val()){
			alert('Carga horária do curso.');
			erro = 1;
			return false;
		}
		if(!$("[name='unoid']").val()){
			alert('Selecione o tipo de unidade de oferta.');
			erro = 1;
			return false;
		}
		if($("#hdn_unidade").val()){
			alert('Selecione a unidade de Oferta.');
			erro = 1;
			return false;
		}
		if(!$("[name='ofeano']").val()){
			alert('Selecione um ano.');
			erro = 1;
			return false;
		}
		if($("#hdn_unidade_selecionada").val() != "1"){
			alert('Selecione a unidade de Oferta.');
			erro = 1;
			return false;
		}
		if(!$("[name='ofeqtde']").val()){
			alert('Informe o número de vagas.');
			erro = 1;
			return false;
		}
		if(erro == 0){
			$("[name='tbn_salvar']").val("Aguarde...");
			$("[name='tbn_salvar']").attr("disabled",true);
			var params = $("#formulario_id").serialize();
			$.ajax({
			url		: 'snf.php?modulo=principal/ofertaCurso&acao=A',
			type	: 'post',
			data	: 'requisicaoAjax=addOferta&'+params,
			success	: function(res){
					$('#div_lista_oferta').html(res);
					$("[name='tbn_salvar']").val("Salvar");
					$("[name='tbn_salvar']").attr("disabled",false);
					$("[name='ofeid']").val("");
					$("#hdn_value_unidade").val("");
					$('#td_unidade select').val($('#hdn_value_unidade').val(""));
					$("[name='ofeqtde']").val("");
				}
			});	
		}
	}
	function unidadeSelecionada(valor)
	{
		if(valor){
			$("[name='hdn_unidade_selecionada']").val("1");
		}else{
			$("[name='hdn_unidade_selecionada']").val("0");
		}
	}
	function cadastrarNovo()
	{
		window.open('snf.php?modulo=principal/cadastroPolo&acao=A&curid=<?php echo $_GET['curid'] ?>&popupoferta=1','Cadastro','scrollbars=no,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
	}
	function alterarOferta(ofeid,tipo,unidade,ofeqtde)
	{
		$("[name='ofeid']").val(ofeid);
		$("[name='unoid']").val(tipo);
		$("#hdn_value_unidade").val(unidade);
		$("[name='ofeqtde']").val(mascaraglobal('[.###]',ofeqtde));
		selecionaUnidade(tipo);
	}
	function excluirOferta(ofeid)
	{
		if(confirm("Deseja realmente excluir?")){
			$.ajax({
			url		: 'snf.php?modulo=principal/ofertaCurso&acao=A',
			type	: 'post',
			data	: 'requisicaoAjax=excluirOferta&ofeid='+ofeid,
			success	: function(res){
					$('#div_lista_oferta').html(res);
				}
			});	
		}
	}
	
	function verificaModalidade(modid) {
		if(modid == '<?=MOD_PRESENCIAL ?>' || modid == '') {
			document.getElementById('tr_chpresencial').style.display = 'none';
			document.getElementById('cargahorariapresencial').value  = '';
		} else {
			document.getElementById('tr_chpresencial').style.display = '';
		}
	}

	function exibeHistoricoDem( curid )
	{
		var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/historico.php' +
			// '?modulo=principal/tramitacao' +
			'?modulo=principal/ofertaCurso' +
			'&acao=A' +
			'&docid=' + curid;
		window.open(
			url,
			'alterarEstado',
			'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
		);
	}
</script>
<?php 
if ( empty($insid) ){
	$sql = "	SELECT ies.insid, insnome, inssigla, inscnpj, instelefone, insfax, insemail
				FROM snf.instituicaoensino ies
				INNER JOIN snf.usuarioresponsabilidade ur ON ur.insid = ies.insid AND rpustatus = 'A'
				WHERE usucpf = '{$_SESSION['usucpf']}'";

	$arrIns = $db->pegaLinha($sql);
	
	$_SESSION['snf']['insid'] = $arrIns['insid'];
}else{
	$sql = "SELECT 
				ies.insid, insnome, inssigla, 
				inscnpj, instelefone, insfax, insemail
			FROM 
				snf.instituicaoensino ies
			where insid = '{$insid}'";
	$arrIns = $db->pegaLinha($sql);
} 
?>
<form name="formulario" id="formulario_id" method="post" action="">
<?php 
echo $htmlComboEstado;
?>

	<input type="hidden" name="requisicao" id="requisicao" value="" />
	<input type="hidden" name="insid" id="insid" value="<?php echo $_SESSION['snf']['insid'] ?>" />
	<input type="hidden" name="curid" id="curid" value="<?php echo $_GET['curid'] ?>" />
	<input type="hidden" name="modid" id="modid" value="<?php echo $_GET['modid'] ?>" />
	<input type="hidden" name="ofeid" id="ofeid" value="" />
	<input type="hidden" name="hdn_value_unidade" id="hdn_value_unidade" value="" />
	<table class="tabela" height="100%" align="center" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>
			<td width="25%" class="subtituloDireita">Orientações</td>
			<td>
				<font color=red>
	Nesta tela é possível realizar a pré-oferta para o curso e o ano escolhidos na tela anterior. Atenção para o ano em que se está realizando a oferta. A oferta em 2012 será aquela referente à demanda de 2012, mas ocorrerá em 2013.
				</font>
			</td>

			<td><?php // workflow
				wf_desenhaBarraNavegacao( $docid , array( ) ); ?></td>

		</tr>

		<tr>
			<td colspan="2" class="subtituloCentro">Dados da Instituição</td>
		</tr>
		<tr>
			<td width="25%" class="subtituloDireita">Nome</td>
			<td>
				<!--   <input type="hidden" name="insid" id="insid" value="<?php echo $arrIns['insid'] ?>" /> -->
				<?php echo $arrIns['insnome'] ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Sigla</td>
			<td><?php echo $arrIns['inssigla'] ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">CNPJ</td>
			<td><?php echo $arrIns['inscnpj'] ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Telefone</td>
			<td><?php echo $arrIns['instelefone'] ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Fax</td>
			<td><?php echo $arrIns['insfax'] ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">E-mail</td>
			<td><?php echo $arrIns['insemail'] ?></td>
		</tr>
		<tr>
			<td colspan="2" class="subtituloCentro">Dados do Curso</td>
		</tr>
		<?php $sql = "	select c.curid, c.curdesc, atedesc, m.moddesc, n.ncudesc, m.modid
						from catalogocurso.curso c 
						inner join catalogocurso.nivelcurso n using(ncuid)
						inner join catalogocurso.modalidadecurso m using(modid)
						inner join catalogocurso.areatematica using(ateid)
						where curstatus = 'A'
						and curid = {$_GET['curid']}";
			  $arrCur = $db->pegaLinha($sql); ?>
		<tr>
			<td class="subtituloDireita">Curso</td>
			<td><?php echo $arrCur['curdesc'] ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Área Temática</td>
			<td><?php echo $arrCur['atedesc'] ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Modalidade</td>
			<td><?php echo $arrCur['moddesc'] ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Nível</td>
			<td><?php echo $arrCur['ncudesc'] ?></td>
		</tr>
		<tr>
			<td colspan="2" class="subtituloCentro">Projeto Pedagógico</td>
		</tr>
		<tr>
			<td class="subtituloDireita">A Instituição possui graduação na área proposta de curso?</td>
			<td>
			<input type="radio" name="possuigraduacao" value="TRUE" <?=(($possuigraduacao=="t")?"checked":"") ?>> Sim <input type="radio" name="possuigraduacao" value="FALSE" <?=(($possuigraduacao=="f")?"checked":"") ?>> Não
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Existe uma política de acesso dos cursistas à biblioteca da Instituição?</td>
			<td>
			<input type="radio" name="politicaacesso" value="TRUE" <?=(($politicaacesso=="t")?"checked":"") ?>> Sim <input type="radio" name="politicaacesso" value="FALSE" <?=(($politicaacesso=="f")?"checked":"") ?>> Não
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Objetivos do curso</td>
			<td>
				<?php echo campo_textarea('objetivocurso','S','S','',80,4,5000) ?>
			</td>
		</tr>
		<!-- 
		<tr>
			<td class="subtituloDireita">Modalidade do curso</td>
			<td><? //$sql = "select modid as codigo, moddesc as descricao from catalogocurso.modalidadecurso where modstatus='A'"; ?>
				<?php //$db->monta_combo('modid', $sql, 'S', 'Selecione', 'verificaModalidade', '', '', '200', 'S', 'modid'); ?>
			</td>
		</tr>
		 -->
		<tr>
			<td class="subtituloDireita">Carga horária do curso</td>
			<td>
				<?php echo campo_texto('cargahoraria', "S", "S", "Carga horária do curso", 6, 5, "#####", "", '', '', 0, 'id="cargahoraria"' ); ?>
			</td>
		</tr>
		<tr id="tr_chpresencial" <?=(($arrCur['modid']==MOD_PRESENCIAL)?'style="display:none;"':'') ?>>
			<td class="subtituloDireita">Carga horária presencial</td>
			<td>
				<?php echo campo_texto('cargahorariapresencial', "S", "S", "Carga horária presencial", 6, 5, "#####", "", '', '', 0, 'id="cargahorariapresencial"' ); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Demanda do Curso por Estado</td>
			<td><table class="listagem" width="60%" ><?php $sql = "	select estuf, sum(privagasprevistas) as qtde
								from snf.prioridadecursoescola 
								where curid = {$_SESSION['snf']['curid']}
							  	and ncuid = {$_SESSION['snf']['ncuid']} 
							  	and modid = {$_SESSION['snf']['modid']} 
							  	and pristatus = 'A'
							  	and pcfid in (1,2)
								group by estuf
								order by estuf";
			  			$arrUF = $db->carregar($sql);
			  	for($i=0;$i<7;$i++){
			  		echo "<tr>";
			  		echo "<td style=\"font-weight:bold;background-color:#c9c9c9\" >".$arrUF[$i]['estuf']."</td>";
			  		echo "<td style=\"text-align:right;color:#0066cc\" >".$arrUF[$i]['qtde']."</td>";
			  		echo "<td style=\"font-weight:bold;background-color:#c9c9c9\" >".$arrUF[$i+7]['estuf']."</td>";
			  		echo "<td style=\"text-align:right;color:#0066cc\" >".$arrUF[$i+7]['qtde']."</td>";
			  		echo "<td style=\"font-weight:bold;background-color:#c9c9c9\" >".$arrUF[$i+14]['estuf']."</td>";
			  		echo "<td style=\"text-align:right;color:#0066cc\" >".$arrUF[$i+14]['qtde']."</td>";
			  		echo "<td style=\"font-weight:bold;background-color:#c9c9c9\" >".$arrUF[$i+21]['estuf']."</td>";
			  		echo "<td style=\"text-align:right;color:#0066cc\" >".$arrUF[$i+21]['qtde']."</td>";
			  		echo "</tr>";
			  	}
			  		
			  	?></table></td>

		</tr>
		<tr>
			<td colspan="2" class="subtituloCentro">Oferta</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Tipo de unidade de oferta </td>
			<td>
				<?php
				$arrUni[0] = array("codigo" => "1","descricao" => "Pólo");
				$arrUni[1] = array("codigo" => "2","descricao" => "Pólo UAB");
				$arrUni[2] = array("codigo" => "3","descricao" => "Campus");
				$db->monta_combo('unoid', $arrUni, $boAtivo, 'Selecione...', 'selecionaUnidade', '', '', '', 'S','unoid');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Unidade de Oferta</td>
			<td id="td_unidade" >Selecione o tipo de unidade de oferta.<input type="hidden" name="hdn_unidade" id="hdn_unidade" value="1" /></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Ano </td>
			<td>
				<?php
				$arrAno[0] = array("codigo" => "2012","descricao" => "2012");
				$arrAno[1] = array("codigo" => "2013","descricao" => "2013");
				$arrAno[2] = array("codigo" => "2014","descricao" => "2014");
				$arrAno[3] = array("codigo" => "2015","descricao" => "2015");
				$arrAno[4] = array("codigo" => "2016","descricao" => "2016");
				$db->monta_combo('ofeano', $arrAno, $boAtivo, 'Selecione...', '', '', '', '', 'S','ofeano');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Número de vagas</td>
			<td><?php echo campo_texto("ofeqtde","S","S","",10,20,"[.###]","") ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita"></td>
			<td><input type="button" name="tbn_salvar" value="Salvar" onclick="addOferta()" /> <input type="button" name="tbn_voltar" value="Voltar" onclick="window.location.href='snf.php?modulo=principal/visualizarDemanda&acao=A'" /></td>
		</tr>
	</table>
</form>
<div id="div_lista_oferta" ><?php listaOfertas() ?></div>