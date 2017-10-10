<?php
include_once(APPRAIZ."academico/classes/MaisMedicos.class.inc");
include_once(APPRAIZ."academico/classes/HospitalUnidade.class.inc");
include_once(APPRAIZ."academico/classes/MaisMedicosResponsabilidade.class.inc");

if($_GET['hsuid']){
	$hsuid = $_GET['hsuid'];
	$_SESSION['academico']['hsuid'] = $hsuid;
}

if(!$_SESSION['academico']['hsuid'])
{
	$arrPerfil = pegaPerfilGeral();
	if(!in_array(PERFIL_MAIS_MEDICOS_UNIDADE,$arrPerfil)){
		$erro_js = "Você não possui perfil para visualizar esta tela.";
	}else{
		$sql = "select hsuid from academico.usuarioresponsabilidade where usucpf = '{$_SESSION['usucpf']}' and pflcod = ".PERFIL_MAIS_MEDICOS_UNIDADE." and rpustatus = 'A'";
		$arrHsuid = $db->carregarColuna($sql);
		if(!$arrHsuid){
			$erro_js = "Você não possui Unidade vinculada.";
		}
		elseif(count($arrHsuid) > 1){
			header("Location: academico.php?modulo=principal/listarMaisMedicosUnidade&acao=A");
		}else{
			$hsuid = $arrHsuid[0];
		}
	}
	
}else{
	$hsuid = $_SESSION['academico']['hsuid'];
}

if($erro_js)
{
	echo "<script>alert('$erro_js');history.back(-1);</script>";
	exit;
}

function cabecalhoUnidadeMaisMedicos($hsuid)
{
	global $db;
	
	$unidade = new HospitalUnidade($hsuid);
	$cabecalho = '<table class="tabela" align="center">'
				   . '	<tr>'
				   . '		<td class="SubTituloEsquerda" style="text-align:right;" >Unidade</td>'
				   . '		<td width="80%" class="SubTituloDireita" style="text-align:left;background:#EEE;"> ' . $unidade->hsudsc . '</td>'
				   . '	</tr>'
				   . '	</table>';
				   
	return $cabecalho;
}

function textoHTML2($hsuid)
{
	?>
<div style="text-align:justify;">
<center>
<h2>PORTARIA NORMATIVA Nº 14, DE 9 DE JULHO DE 2013</h2>
<p><b>MINISTÉRIO DA EDUCAÇÃO</b></p>
<p><b>GABINETE DO MINISTRO</b></p>
<p><b>DOU de 10/07/2013 (nº 131, Seção 1, pág. 18)</b></p>
</center>
<p>Dispõe sobre os procedimentos de adesão das instituições federais de educação superior ao Projeto Mais Médicos e dá outras providências.</p>
<p>O MINISTRO DE ESTADO DA EDUCAÇÃO no uso da atribuição que lhe confere o art. 87, inciso II da Constituição Federal, e tendo em vista o disposto na Medida Provisória nº 621, de 8 de julho de 2013, bem como na Portaria Interministerial MS/MEC nº 1.369, de 8 de julho de 2013, resolve:</p>
<p>Art. 1º - Poderão aderir ao Projeto Mais Médicos as instituições federais de educação superior que ofereçam curso de Medicina.</p>
<p>§ 1º - As instituições federais de educação superior interessadas em aderir ao Projeto Mais Médicos deverão apresentar termo de pré-adesão, conforme o modelo do <a href="javascript:abreLinkAnexo(<?php echo $hsuid ?>)" >Anexo I</a> desta Portaria, no período de 11 a 15 de julho de 2013, ao Ministério da Educação.</p>
<p>§ 2º - As instituições deverão indicar, no momento da préadesão, um tutor acadêmico responsável pelas atividades e, no mínimo, três tutores acadêmicos para fins de cadastro de reserva, que atendam aos requisitos da Portaria Interministerial MS/MEC nº 1.369, de 8 de julho de 2013 e desta Portaria.</p>
<p>§ 3º - As instituições deverão cadastrar via sistema <?php echo SIGLA_SISTEMA; ?>, no módulo rede federal, por meio do endereço eletrônico <a href="http://simec.mec.gov.br" target="_blank">http://simec.mec.gov.br</a>, os tutores indicados no termo de pré-adesão.</p>
<p>§ 4º - No momento da pré-adesão as instituições deverão indicar a unidade responsável pela avaliação e autorização de pagamento das bolsas de tutoria e supervisão acadêmicas.</p>
<p>Art. 2º - O Ministério da Educação decidirá sobre a validação do termo de pré-adesão das instituições que atenderem aos requisitos previstos no art. 1º desta Portaria, observadas as necessidades do Projeto Mais Médicos.</p>
<p>Parágrafo único - Em caso de manifestação de interesse de mais de uma instituição por unidade da federação, será dada preferência àquela sediada na capital, caso persista o empate, será selecionada àquela que ofertar curso de Medicina há mais tempo.</p>
<p>Art. 3º - As instituições que tiverem seus termos de pré-adesão validados pelo Ministério da Educação deverão firmar termo de adesão no prazo máximo de 10 (dez) dias após a divulgação das instituições selecionadas.</p>
<p>Parágrafo único - O termo de adesão estará disponível para assinatura das instituições selecionadas no sistema <?php echo SIGLA_SISTEMA; ?>, no módulo rede federal, por meio do endereço eletrônico <a href="http://simec.mec.gov.br" target="_blank">http://simec.mec.gov.br</a>, e conterá, no mínimo, as seguintes obrigações para a instituição:</p>
<p>I - atuar em cooperação com os entes federativos, as Coordenações Estaduais do Projeto e organismos internacionais, no âmbito de sua competência, para execução do Projeto Mais Médicos;</p>
<p>II - coordenar o acompanhamento acadêmico do Projeto;</p>
<p>III - ratificar a unidade responsável pela avaliação e autorização de pagamento das bolsas de tutoria e supervisão acadêmicas, indicada no termo de pré-adesão;</p>
<p>IV - definir mecanismo de avaliação e autorização de pagamento das bolsas de tutoria e supervisão;</p>
<p>V - ratificar a indicação dos tutores acadêmicos do Projeto, feita no termo de pré-adesão;</p>
<p>VI - definir critérios e mecanismo de seleção de supervisores;</p>
<p>VII - realizar seleção dos supervisores do Projeto;</p>
<p>VIII - monitorar e acompanhar as atividades dos supervisores e tutores acadêmicos no âmbito do Projeto;</p>
<p>IX - ofertar os módulos de acolhimento e avaliação aos médicos intercambistas; e</p>
<p>X - ofertar cursos de especialização e atividades de pesquisa, ensino e extensão aos médicos participantes.</p>
<p>Art. 4º - Os tutores acadêmicos serão selecionados pela instituição entre os docentes da área médica, preferencialmente vinculados à área de saúde coletiva ou correlata, ou à área de clínica médica.</p>
<p>§ 1º - Os tutores acadêmicos perceberão bolsa-tutoria, na forma prevista no termo de adesão.</p>
<p>§ 2º - Os tutores acadêmicos serão responsáveis pela orientação acadêmica e pelo planejamento das atividades do supervisor, trabalhando em parceria com as Coordenações Estaduais do Projeto, e tendo, no mínimo, as seguintes atribuições:</p>
<p>I - coordenar as atividades acadêmicas da integração ensinoserviço, atuando em cooperação com os supervisores e os gestores do SUS;</p>
<p>II - indicar, em plano de trabalho, as atividades a serem executadas pelos médicos participantes e supervisores, bem como a metodologia de acompanhamento e avaliação;</p>
<p>III - monitorar o processo de acompanhamento e avaliação a ser executado pelos supervisores, garantindo sua continuidade;</p>
<p>IV - integrar as atividades do curso de especialização às atividades de integração ensino-serviço;</p>
<p>V - relatar à instituição pública de ensino superior à qual esteja vinculado a ocorrência de situações nas quais seja necessária a adoção de providência pela instituição; e</p>
<p>VI - apresentar relatórios periódicos da execução de suas atividades no Projeto à instituição à qual esteja vinculado e à Coordenação do Projeto.</p>
<p>Art. 5º - Os supervisores serão selecionados entre profissionais médicos por meio de edital conforme critérios e mecanismos estabelecidos pela instituição aderente e validados pela Coordenação Estadual do Projeto Mais Médicos.</p>
<p>§ 1º - Os supervisores selecionados perceberão bolsa, conforme avaliação e autorização das instituições aderentes, na forma prevista no termo de adesão.</p>
<p>§ 2º - Os supervisores selecionados serão responsáveis pelo acompanhamento e fiscalização das atividades de ensino-serviço do médico participante, em conjunto com o gestor do SUS no Município, e terão, no mínimo, as seguintes atribuições:</p>
<p>I - realizar visita periódica para acompanhar atividades dos médicos participantes;</p>
<p>II - estar disponível para os médicos participantes, por meio de telefone e internet;</p>
<p>III - aplicar instrumentos de avaliação presencialmente; e</p>
<p>IV - acompanhar e fiscalizar, em conjunto com o gestor do SUS, o cumprimento da carga horária de 40 horas semanais prevista pelo Projeto para os médicos participantes, por meio de sistema de informação disponibilizado pela Coordenação do Programa.</p>
<p>Art. 6º - Esta Portaria entra em vigor na data de sua publicação.</p>
<p>ALOIZIO MERCADANTE OLIVA</p>
<?php
}

function recuperaArquivo($arqid)
{
	global $db;
	$sql = "select * from public.arquivo where arqid = $arqid";
	return $db->pegaLinha($sql);
}

if($_GET['arquivo'])
{
	include_once APPRAIZ . "includes/classes/file.class.inc";
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
	$file-> getDownloadArquivo($_GET['arquivo']);
	exit;
}
 
if($_REQUEST['requisicaoAjax'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicaoAjax']();
	die;
}
if($_REQUEST['requisicao'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicao']();
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

monta_titulo( "Mais Médicos", "");

// cria o cabeçalho padrão do módulo
echo cabecalhoUnidadeMaisMedicos($hsuid);

$_SESSION['academico']['hsuid'] = $hsuid;

$maisMedicos = new MaisMedicos();
$arrDados = $maisMedicos->recupaPorUnidade($hsuid);
if($arrDados){
	extract($arrDados);
}
if($maisMedicos->msminteresse != "t"){
	$exibe_termo_aceite = true;
}else{
	$exibe_termo_aceite = false;
}
?>
<script>
var verificaPendencias=true;
function abreTermo(){

	return windowOpen('academico.php?modulo=principal/termo&acao=A', 'blank', 'height=600,width=750,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
}
</script>
<style>.link{cursor:pointer}</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
<form name="formulario" id="formulario" method="post" action="" enctype="multipart/form-data" >
<input type="hidden" id="requisicao" name="requisicao" value="salvarMaisMedicos" />
<input type="hidden" id="classe" name="classe" value="MaisMedicos" />
<input type="hidden" id="msminteresse" name="msminteresse" value=""" />
<input type="hidden" id="msmid" name="msmid" value="<?php echo $maisMedicos->msmid?>" />
<input type="hidden" id="hsuid" name="hsuid" value="<?php echo $hsuid?>" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<?php if($exibe_termo_aceite): ?>
		<tr>
			<td colspan="2" align="center" >
				<div style="width:90%;height:400px;border:solid 1px black;overflow:auto;padding:3px;background-color:#fff">
				<?php echo textoHTML2($hsuid) ?>
				</div>
			</td>
		</tr>
		<tr>
			<td colspan="2" align="center">
				<?php $data = date("Ymd")?>
				<?php if( ($data >= 20130711 && $data <= 20130725) || $db->testa_superuser()):?>
					<input type="button" name="btn_aceitar" value="Realizar Pré-Adesão" onclick="aceitarTermo()" />
				<?php else: ?>
					A data para realizar a adesão ao Programas Mais Médicos ocorreu entre os dias 11 e 25 de julho de 2013. Aguadem novas informações.
				<?php endif; ?>
				<?php if($_GET['hsuid']): ?>
	    			<input type="button" name="btn_voltar" value="Voltar" onclick="window.location.href='academico.php?modulo=principal/listarMaisMedicosUnidade&acao=A'" />
	    		<?php endif;?>
			</td>
		</tr>
	<?php else: ?>
		<tr>
			<td class="subtituloCentro" colspan="2">Órgão de Avaliação e Autorização de Pagamentos - <a onclick="abreLinkAnexo(<?php echo $hsuid ?>)" >Abrir Termo</a>
			</td>
		</tr>
		<tr>
			<td colspan="2" align="center" >
				<p>A unidade de avaliação e autorização será responsável pelo acompanhamento do trabalho realizado pelos tutores, supervisores e os médicos, sendo a principal responsável pelas autorizações mensais dos pagamentos das bolsas recebidas pelos tutores do programa.</p>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width='250px;' >Nome da Unidade de Avaliação e Autorização de Pagamentos</td>
			<td>
				<?php echo campo_texto('msmsetorresponsavel', 'S', "S", '', 60, 255, '', '', 'left', '', 0, 'id="msmsetorresponsavel"', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" >Telefone</td>
			<td>
				<?php echo campo_texto('msmdddsetor', 'N', "S", '', 2, 2, '##', '', 'left', '', 0, 'id="msmdddsetor"', ''); ?> - <?php echo campo_texto('msmfonesetor', 'S', "S", '', 10, 10, '####-####', '', 'left', '', 0, 'id="msmfonesetor"', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" >Pessoa Responsável</td>
			<td>
				<?php echo campo_texto('msmresponsavelsetor', 'S', "S", '', 60, 255, '', '', 'left', '', 0, 'id="msmresponsavelsetor"', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" >Email do Setor Responsável</td>
			<td>
				<?php echo campo_texto('msmemailsetor', 'S', "S", '', 60, 255, '', '', 'left', '', 0, 'id="msmemailsetor"', ''); ?>
			</td>
		</tr>
		<tr>
    		<td colspan="2" class="SubtituloCentro" >Anexo do Termo de Pré-Adesão</td>
		</tr>
		<tr>
			<td colspan="2" align="center" >
				<p>Favor anexar abaixo o <A href="javascript:abreLinkAnexo(<?php echo $hsuid ?>);" >termo de pré-adesão</a> assinado.</p>
			</td>
		</tr>
		<tr>
	    	<td class="SubTituloDireita">Termo de Pré-Adesão Assinado</td>
	        <td id="td_arquivo">
			<?php if($arqid): ?>
				<?php $arrArquivo = recuperaArquivo($arqid)  ?>
				<a href="javascript:downloadAnexo(<?php echo $arqid ?>)" ><?php echo $arrArquivo['arqnome'] ?>.<?php echo $arrArquivo['arqextensao'] ?></a>
				<?php if(!$somente_leitura): ?>
					<img src="../imagens/excluir.gif" class="link" onclick="excluirAnexo(<?php echo $arqid ?>)" />
				<?php endif; ?>
				<input type="hidden" name="arquivo" id="arquivo" value="<?php echo $arqid ?>" />
			<?php else: ?>
				<input type="file" name="arquivo" id="arquivo" />
			<?php endif; ?> 
			</td>
		</tr>
		<tr>
    		<td colspan="2" class="SubtituloCentro" >Tutor(es)</td>
		</tr>
		<tr>
    		<td colspan="2" >
    			<span title="Cadastrar Novo Tutor" class="link" style="font-weight:bold" onclick="cadNovoTutor()"><img style="vertical-align:middle" src="../imagens/gif_inclui.gif" /> Novo Tutor</span>
    		</td>
		</tr>
	<?php endif; ?>
</table>
</form>
<div id="div_lista_tutores">
<center>
<?php 
	if(!$exibe_termo_aceite){
		$tutor = new MaisMedicosResponsabilidade();
		$tutor->listaTutores(null,$hsuid);
	}
?>
</center>
</div>
<?php if(!$exibe_termo_aceite): ?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
	    	<td class="subtituloCentro" colspan="2" >
	    		<input type="button" name="btn_salvar" value="Salvar" onclick="salvarMaisMedicos()" />
	    		<?php if($_GET['hsuid']): ?>
	    			<input type="button" name="btn_voltar" value="Voltar" onclick="window.location.href='academico.php?modulo=principal/listarMaisMedicosUnidade&acao=A'" />
	    		<?php endif;?>
	    	</td>
		</tr>
	</table>
<?php endif; ?>
<script>
function aceitarTermo()
{
	$("#msminteresse").val("sim");
	$("#classe").val("MaisMedicos");
	$("#requisicao").val("salvarMaisMedicos");
	$("#formulario").submit();
}

function cadNovoTutor()
{
	return windowOpen('academico.php?modulo=principal/cadTutoresUnidade&acao=A','blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

function alterarTutor(entid)
{
	return windowOpen('academico.php?modulo=principal/cadTutoresUnidade&acao=A&entid='+entid,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

function salvarMaisMedicos()
{
	var erro = 0;
	$("[class~=obrigatorio]").each(function() { 
		if(!this.value || this.value == "Selecione..."){
			erro = 1;
			alert('Favor preencher todos os campos obrigatórios.');
			this.focus();
			return false;
		}
	});
	/*if(erro == 0 && !$("#arquivo").val()){
		erro = 1;
		alert('Favor informar o Anexo do Termo de Pré-Adesão.');
		return false;
	}*/
	if(erro == 0){
		$("#classe").val("MaisMedicos");
		$("#requisicao").val("salvarMaisMedicos");
		$("#formulario").submit();
	}
}

function excluirTutor(mmrid,hsuid)
{
	if(confirm("Deseja realmente exluir este tutor?"))
	{
		$.ajax({
		   type: "POST",
		   url: window.location,
		   data: "requisicaoAjax=excluirTutor&classe=MaisMedicosResponsabilidade&mmrid="+mmrid+"&hsuid="+hsuid,
		   success: function(msg){
			   	alert('Operação realizada com sucesso.');
		   		$('#div_lista_tutores').html( msg );
		   }
		 });
	}
}

function listaTutoresAjax(hsuid)
{
	$.ajax({
	   type: "POST",
	   url: window.location,
	   data: "requisicaoAjax=listaTutoresAjax&classe=MaisMedicosResponsabilidade&hsuid="+hsuid,
	   success: function(msg){
	   		$('#div_lista_tutores').html( msg );
	   }
	 });
}

function atribuirTitularidade(entidtutor,hsuid)
{
	$.ajax({
		   type: "POST",
		   url: window.location,
		   data: "requisicaoAjax=atribuirTitularidade&classe=MaisMedicosResponsabilidade&entidtutor="+entidtutor+"&hsuid="+hsuid,
		   success: function(msg){
			   	//alert('Operação realizada com sucesso.');
		   }
	});
}

function abreLinkAnexo(hsuid)
{
	return windowOpen('academico.php?modulo=principal/anexoPortaria&acao=A&hsuid='+hsuid,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

function downloadAnexo(arqid)
{
	window.location.href="academico.php?modulo=principal/maisMedicos&acao=A&arquivo="+arqid;
}

function excluirAnexo(arqid)
{
	if(confirm("Deseja realmente exluir este arquivo?"))
	{
		$.ajax({
		   type: "POST",
		   url: window.location,
		   data: "requisicaoAjax=excluirAnexo&classe=MaisMedicos&arqid="+arqid,
		   success: function(msg){
		   		$("#arquivo").val("");
		   		$('#td_arquivo').html( "<input type=\"file\" name=\"arquivo\" id=\"arquivo\" />" );
		   }
		 });
	}
}

$(function() {
	<?php if($_SESSION['academico']['maismedicos']['alert']): ?>
		alert('<?php echo $_SESSION['academico']['maismedicos']['alert'] ?>');
		<?php unset($_SESSION['academico']['maismedicos']['alert']) ?>
	<?php endif; ?>
});
</script>