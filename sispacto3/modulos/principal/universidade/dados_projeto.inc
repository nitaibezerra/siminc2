<?

verificarTermoCompromisso(array("iusd"=>$_SESSION['sispacto3']['universidade']['iusd']));

$estado = wf_pegarEstadoAtual( $_SESSION['sispacto3']['universidade']['dociddadosprojeto'] );

if($estado['esdid'] != ESD_ELABORACAO_COORDENADOR_IES) {
	$consulta = true;
}

$info = carregarCadastroIESProjeto(array("uncid"=>$_SESSION['sispacto3']['universidade']['uncid']));
extract($info['universidade']);
extract($info['curso']);

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/pj.js" /></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>


<script>

<? if($consulta) : ?>
jQuery(function() {
jQuery("[name='salvar']").css('display','none');
jQuery("[name='salvarcontinuar']").css('display','none');
jQuery("[name='importar']").css('display','none');

jQuery("[type='text']").attr('class','disabled');
jQuery("[type='text']").attr('disabled','disabled');
jQuery("[name='unctipo'],[name='unctipocertificacao']").attr('disabled','disabled');
});
<? endif; ?>

function exibirInformacoes(msg) {
	alert(msg);
}

function salvarDadosIES(goto) {

	if(jQuery('#unicnpj').val()=='') {
		alert('CNPJ em branco');
		return false;
	}
	
	if(!validarCnpj(jQuery('#unicnpj').val())) {
		alert('CNPJ inválido');
		return false;
	}
	
	if(jQuery('#uninome').val()=='') {
		alert('Nome da Instituição em branco');
		return false;
	}
	
	if(jQuery('#unisigla').val()=='') {
		alert('Sigla em branco');
		return false;
	}
	
	if(jQuery('#unicep').val()=='') {
		alert('CEP em branco');
		return false;
	}
	
	if(jQuery('#unicep').val().length!=9) {
		alert('CEP inválido');
		return false;
	}
	
	if(jQuery('#uniuf').val()=='') {
		alert('UF em branco');
		return false;
	}

	if(jQuery('#muncod_endereco').val()=='') {
		alert('Município em branco');
		return false;
	}
	
	if(jQuery('#unilogradouro').val()=='') {
		alert('Logradouro em branco');
		return false;
	}
	
	if(jQuery('#unibairro').val()=='') {
		alert('Bairro em branco');
		return false;
	}
	
	if(jQuery('#uninumero').val()=='') {
		alert('Número em branco');
		return false;
	}
	
	if(jQuery('#unidddcomercial').val()=='') {
		alert('DDD em branco');
		return false;
	}
	
	if(jQuery('#uninumcomercial').val()=='') {
		alert('Telefone em branco');
		return false;
	}
	
	if(jQuery('#uniemail').val()=='') {
		alert('Email em branco');
		return false;
	}
	
	if(jQuery('#unisite').val()=='') {
		alert('Site em branco');
		return false;
	}
	
	if(jQuery('#reicpf').val()=='') {
		alert('CPF em branco');
		return false;
	}
	
	if(!validar_cpf(jQuery('#reicpf').val())) {
		alert('CPF inválido');
		return false;
	}
	
	if(jQuery('#reinome').val()=='') {
		alert('Nome em branco');
		return false;
	}
	
	if(jQuery('#reidddcomercial').val()=='') {
		alert('DDD em branco');
		return false;
	}
	
	if(jQuery('#reinumcomercial').val()=='') {
		alert('Telefone em branco');
		return false;
	}
	
	if(jQuery('#reiemail').val()=='') {
		alert('Email em branco');
		return false;
	}

	if(jQuery('#uncdatainicioprojeto').val()=='') {
		alert('Início - Vigência do projeto em branco');
		return false;
	}
	
	if(!validaData(document.getElementById('uncdatainicioprojeto'))) {
		alert('Início - Vigência do projeto inválida');
		return false;
	}

	if(jQuery('#uncdatafimprojeto').val()=='') {
		alert('Fim - Vigência do projeto em branco');
		return false;
	}
	
	if(!validaData(document.getElementById('uncdatafimprojeto'))) {
		alert('Fim - Vigência do projeto inválida');
		return false;
	}
	
	var data1    = jQuery('#uncdatainicioprojeto').val();
	data1 		 = data1.split('/');
	var data2    = jQuery('#uncdatafimprojeto').val();
	data2 		 = data2.split('/');
	
	if(data2[2]+data2[1]+data2[0] < data1[2]+data1[1]+data1[0]) {
		alert('Data de Inicio de Vigência do projeto é maior do que Data Fim');
		return false;
	}
	
	if(jQuery('#unctipo').val()=='') {
		alert('Origem dos recursos em branco.');
		return false;
	}
	
    divCarregando();
    
    jQuery('#goto').val(goto);

	document.getElementById('formulario').submit();

}

function habilitaReitor() {
	jQuery("#reicpf").attr('disabled',false);
	jQuery("#reicpf").attr('readonly','');
	jQuery("#reicpf").attr('class','obrigatorio normal');

	jQuery("#reinome").attr('disabled',false);

	jQuery("#reidddcomercial").attr('disabled',false);
	jQuery("#reidddcomercial").attr('readonly','');
	jQuery("#reidddcomercial").attr('class','obrigatorio normal');
	
	jQuery("#reinumcomercial").attr('disabled',false);
	jQuery("#reinumcomercial").attr('readonly','');
	jQuery("#reinumcomercial").attr('class','obrigatorio normal');
	
	jQuery("#reiemail").attr('disabled',false);
	jQuery("#reiemail").attr('readonly','');
	jQuery("#reiemail").attr('class','obrigatorio normal');


	var input = document.createElement("input");
	input.setAttribute("type", "hidden");
	input.setAttribute("name", "somentereitor");
	input.setAttribute("value", "1");
	document.getElementById("formulario").appendChild(input);

	jQuery("[name='salvar']").css('display','');
	
}

function carregaCPF() {
    divCarregando();
	var usucpf=document.getElementById('reicpf').value;
	usucpf = usucpf.replace('-','');
	usucpf = usucpf.replace('.','');
	usucpf = usucpf.replace('.','');
			
	var comp = new dCPF();
	comp.buscarDados(usucpf);
	var arrDados = new Object();
	if(!comp.dados.no_pessoa_rf){
		alert('CPF Inválido');
  		divCarregado();
		return false;
	}
	document.getElementById('reinome').value=comp.dados.no_pessoa_rf;
    divCarregado();
}

function carregaCNPJ() {
    divCarregando();
	var cnpj = document.getElementById('unicnpj').value;
	cnpj = cnpj.replace('-','');
	cnpj = cnpj.replace('.','');
	cnpj = cnpj.replace('/','');
			
	var comp = new dCNPJ();
	comp.buscarDados(cnpj);
	
	var arrDados = new Object();
	if(!comp.dados.no_empresarial_rf){
		alert('CNPJ Inválido');
  		divCarregado();
		return false;
	}
	if(comp.dados.nu_cep) {
		document.getElementById('unicep').value = mascaraglobal('#####-###',comp.dados.nu_cep);
		carregarEnderecoPorCEP_dirigente(comp.dados.nu_cep);
	}
	document.getElementById('uninome').value=comp.dados.no_empresarial_rf;
    divCarregado();
}
</script>
<form method="post" id="formulario">
<input type="hidden" name="requisicao" value="atualizarDadosIES">
<input type="hidden" name="uncid" value="<?=$uncid ?>">
<input type="hidden" name="goto" id="goto" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Dados Instituição/Projeto</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
	</tr>
	<tr>
		<td colspan="2" align="right"><input type="button" name="importar" value="Importar SISPACTO 2014" onclick="importarInformacoesSispacto('dados_projeto');"></td>
	</tr>
	
	<tr>
		<td colspan="2">
		
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Dados da Instituição</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">CNPJ</td>
		<td><?=campo_texto('unicnpj', "S", "S", "CNPJ", 20, 20, "##.###.###/####-##", "", '', '', 0, 'id="unicnpj"', '', mascaraglobal($unicnpj,"##.###.###/####-##"),'if(this.value.length==18){carregaCNPJ();}'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Nome da Instituição</td>
		<td><?=campo_texto('uninome', "S", "N", "Nome da Instituição", 67, 150, "", "", '', '', 0, 'id="uninome"', '', $uninome); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Sigla</td>
		<td><?=campo_texto('unisigla', "S", "S", "sigla", 15, 10, "", "", '', '', 0, 'id="unisigla"', '', $unisigla); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Endereço</td>
		<td>
			<table cellSpacing="1" cellPadding="3" width="100%">
				<tr>
					<td align="right" width="10%">CEP</td>
					<td><?=campo_texto('unicep', "S", "S", "CEP", 9, 10, "#####-###", "", '', '', 0, 'id="unicep"', '', mascaraglobal($unicep,"#####-###"),'if(this.value.length==9){carregarEnderecoPorCEP_dirigente(comp.dados.nu_cep);}'); ?></td>
				</tr>
				<tr>
					<td align="right">UF</td>
					<td><?=(($uniuf)?$uniuf."<input type=\"hidden\" name=\"uniuf\" id=\"uniuf\" value=\"{$uniuf}\">":"Digite CEP") ?></td>
				</tr>
				<tr>
					<td align="right">Município</td>
					<td id="td_municipio_dirigente"><?=(($muncod)?$mundescricao."<input type=\"hidden\" id=\"muncod_endereco\" name=\"muncod_endereco\" value=\"".$muncod."\">":"Digite CEP") ?></td>
				</tr>
				<tr>
					<td align="right">Logradouro</td>
					<td><?=campo_texto('unilogradouro', "S", "S", "Logradouro", 60, 150, "", "", '', '', 0, 'id="unilogradouro"', '', $unilogradouro ); ?></td>
				</tr>
				<tr>
					<td align="right">Bairro</td>
					<td><?=campo_texto('unibairro', "S", "S", "Bairro", 60, 150, "", "", '', '', 0, 'id="unibairro"', '', $unibairro ); ?></td>
				</tr>
				<tr>
					<td align="right">Complemento</td>
					<td><?=campo_texto('unicomplemento', "N", "S", "Complemento", 60, 150, "", "", '', '', 0, 'id="unicomplemento"', '', $unicomplemento ); ?></td>
				</tr>
				<tr>
					<td align="right">Número</td>
					<td><?=campo_texto('uninumero', "S", "S", "Número", 6, 5, "#####", "", '', '', 0, 'id="uninumero"', '', $uninumero ); ?></td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Contato</td>
		<td>
			<table cellSpacing="1" cellPadding="3" width="100%">
				<tr>
					<td align="right" width="10%">Telefone</td>
					<td><?=campo_texto('unidddcomercial', "N", "S", "DDD", 3, 3, "##", "", '', '', 0, 'id="unidddcomercial"', '', $unidddcomercial ); ?> <?=campo_texto('uninumcomercial', "S", "S", "Telefone", 20, 20, "####-####", "", '', '', 0, 'id="uninumcomercial"', '', $uninumcomercial ); ?></td>
				</tr>
				<tr>
					<td align="right" width="10%">E-mail</td>
					<td><?=campo_texto('uniemail', "S", "S", "E-mail", 60, 60, "", "", '', '', 0, 'id="uniemail"', '', $uniemail); ?></td>
				</tr>
				<tr>
					<td align="right" width="10%">Site</td>
					<td><?=campo_texto('unisite', "S", "S", "Site", 60, 60, "", "", '', '', 0, 'id="unisite"', '', $unisite); ?></td>
				</tr>
			</table>
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Dados do Dirigente
		<? 
		$perfis = pegaPerfilGeral(); 
		if($consulta && ($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis))) : 
		?>
		<img src="../imagens/principal.gif" align="absmiddle" style="cursor:pointer;" onclick="habilitaReitor();">
		<? 
		endif; 
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">CPF</td>
		<td><?=campo_texto('reicpf', "S", "S", "CPF", 17, 14, "###.###.###-##", "", '', '', 0, 'id="reicpf"', '', mascaraglobal($reicpf,"###.###.###-##"),'if(this.value.length==14){carregaCPF();}'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Nome</td>
		<td><?=campo_texto('reinome', "S", "N", "Nome", 67, 150, "", "", '', '', 0, 'id="reinome"', '', $reinome); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Telefone</td>
		<td><?=campo_texto('reidddcomercial', "N", "S", "DDD", 3, 3, "##", "", '', '', 0, 'id="reidddcomercial"', '', $reidddcomercial ); ?> <?=campo_texto('reinumcomercial', "S", "S", "Telefone", 20, 20, "####-####", "", '', '', 0, 'id="reinumcomercial"', '', $reinumcomercial ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">E-mail</td>
		<td><?=campo_texto('reiemail', "S", "S", "E-mail", 60, 60, "", "", '', '', 0, 'id="reiemail"', "", $reiemail); ?></td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Dados do Projeto</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Curso</td>
			<td><?=$curid." - ".$curdesc ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Objetivo do Curso</td>
		<td><?=$curobjetivo ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Descrição do Curso</td>
		<td><?=$curementa ?></td>
	</tr>	
	
	<tr>
		<td class="SubTituloDireita" width="20%">Meta física</td>
		<td>
		<?
		
		$orientadorestudo = totalAlfabetizadoresAbrangencia(array("uncid" => $_SESSION['sispacto3']['universidade']['uncid']));
		
		$sql = "SELECT SUM(total) FROM sispacto3.totalalfabetizadores t 
				INNER JOIN sispacto3.abrangencia ap ON ap.muncod = t.cod_municipio AND ap.esfera='M'
		 		INNER JOIN sispacto3.estruturacurso e ON e.ecuid = ap.ecuid 
				WHERE e.uncid='".$_SESSION['sispacto3']['universidade']['uncid']."'";
		
		$professoralfabetizador = $db->pegaUm($sql);
		
		
		// Adicionando números referentes ao estadual
		
		$sql = "SELECT count(distinct pa.iusd) as professorestadual, count(distinct i.iusd) as orientadorestadual FROM sispacto3.identificacaousuario i
				INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=1381
				INNER JOIN sispacto3.pactoidadecerta o ON o.picid = i.picid AND o.estuf IS NOT NULL
				INNER JOIN sispacto3.abrangencia ap ON ap.muncod = i.muncodatuacao and ap.esfera='E'
				INNER JOIN sispacto3.estruturacurso e ON e.ecuid = ap.ecuid
				INNER JOIN sispacto3.turmas tu ON tu.iusd = i.iusd
				INNER JOIN sispacto3.professoralfabetizadorturma pa ON pa.turid = tu.turid
				WHERE e.uncid='".$_SESSION['sispacto3']['universidade']['uncid']."'";
		
		$pfestaduais = $db->pegaLinha($sql);
		
		$orientadorestudo       += $pfestaduais['orientadorestadual'];
		$professoralfabetizador += $pfestaduais['professorestadual'];
		
		?>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
			<tr>
				<td class="SubTituloDireita" width="30%">Orientador de Estudo Formado e Certificado</td>
				<td align="right"><?=(($orientadorestudo)?round($orientadorestudo*0.8):"0") ?> <span style="cursor:pointer;font-size:xx-small;" onclick="exibirInformacoes('Orientadores de estudo - A meta física representa um percentual sobre o número total de Orientadores de Estudo das redes alcançadas pelo projeto da IES. Este número só será exibido depois que for definida a área de abrangência do projeto, que será indicada na tela seguinte (Estrutura de curso). O valor exibido corresponde a 80% do somatório de Orientadores de Estudo da área de abrangência. Por exemplo: se a IES irá atuar em 100 municípios que, juntos, somam 300 Orientadores de Estudo, a meta física neste caso corresponderá a 300 x 0,8 = 240 Orientadores de Estudo formados e certificados.');"><img src="../imagens/ajuda.png" width="10" height="10"> Entenda este número</span></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="30%">Professor alfabetizador certificado</td>
				<td align="right"><?=(($professoralfabetizador)?round($professoralfabetizador*0.6):"0") ?> <span style="cursor:pointer;font-size:xx-small;" onclick="exibirInformacoes('Professores Alfabetizadores - A meta física representa um percentual sobre o número total de turmas de alfabetização das redes alcançadas pelo projeto da IES. Este número só será exibido depois que for definida a área de abrangência do projeto, que será indicada na tela seguinte (Estrutura de curso). O valor exibido corresponde a 60% do somatório das turmas de alfabetização da área de abrangência (de acordo com o Censo Escolar preliminar de 2012). Por exemplo: se a IES irá atuar em 100 municípios que, juntos, somam 6.000 turmas de alfabetização, a meta física neste caso corresponderá a 6.000 x 0,6 = 3.600 professores alfabetizadores certificados.');"><img src="../imagens/ajuda.png" width="10" height="10"> Entenda este número</span></td>
			</tr>
		</table>
		</td>
	</tr>

	<tr>
		<td class="SubTituloDireita" width="20%">Vigência do projeto</td>
		<td>
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" width="100%">
				<tr>
					<td class="SubTituloCentro">Início</td>	
					<td class="SubTituloCentro">Término</td>			
				</tr>
				<tr>
					<td>
						<?=campo_data2('uncdatainicioprojeto','S', 'S', 'Início', 'S', '', '', '', '', '', 'uncdatainicioprojeto'); ?>
					</td>
					<td>
						<?=campo_data2('uncdatafimprojeto','S', 'S', 'Término', 'S', '', '', '', '', '', 'uncdatafimprojeto'); ?>
					</td>
				</tr>
			</table>
		</td>
	</tr>	

	<tr>
		<td class="SubTituloDireita" width="20%">Público Alvo</td>
		<td>
		<?
		$sql = "select fexdesc from catalogocurso.funcaoexercida_curso_publicoalvo fcp
				inner join catalogocurso.funcaoexercida fex on fex.fexid = fcp.fexid and fex.fexstatus = 'A'
				where curid=".$curid;
		
		$db->monta_lista_simples($sql,array(),50,5,'N','100%',$par2);
		?>
		</td>
	</tr>		
	<tr>
		<td class="SubTituloDireita" width="20%">Requisitos para Participação</td>
		<td>
		<?
		$sql = "SELECT tee.no_etapa_ensino
				FROM catalogocurso.etapaensino_curso eec
				left join catalogocurso.etapaensino ee on ee.eteid = eec.eteid
				left join educacenso_2010.tab_etapa_ensino tee on tee.pk_cod_etapa_ensino = eec.cod_etapa_ensino
				where curid =".$curid;
		
		$db->monta_lista_simples($sql,array(),50,5,'N','100%',$par2);
		
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Origem dos recursos</td>
		<td><? 
		$_TIPO_AO = array(0 => array("codigo"=>"A","descricao"=>"Ação Orçamentária"),
						  1 => array("codigo"=>"D","descricao"=>"Descentralização"),
						  2 => array("codigo"=>"C","descricao"=>"Convênio"),
						  3 => array("codigo"=>"P","descricao"=>"Plano de Ações Articuladas - PAR"));
		$db->monta_combo('unctipo', $_TIPO_AO, 'S', 'Selecione', '', '', '', '', 'S', 'unctipo'); 
		?></td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" width="20%">Carga Horária</td>
		<td>
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" width="100%">
				<tr>
					<td class="SubTituloCentro">Minimo</td>	
					<td class="SubTituloCentro">Máximo</td>			
				</tr>
				<tr>
					<td><?=$curchmim ?></td>
					<td><?=$curchmax ?></td>
				</tr>
			</table>
		</td>		
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Tipo de Certificação</td>
		<td><? 	
		$_TIPO_CT = array(0 => array("codigo"=>"E","descricao"=>"Extensão"),
						  1 => array("codigo"=>"A","descricao"=>"Aperfeiçoamento"));
		$db->monta_combo('unctipocertificacao', $_TIPO_CT, 'S', 'Selecione', '', '', '', '', 'S', 'unctipocertificacao'); 
		 ?></td>
	</tr>
	
	</table>
	
	</td>
	
	<td valign="top" width="5%">
		<?
		/* Barra de estado atual e ações e Historico */
		wf_desenhaBarraNavegacao( $_SESSION['sispacto3']['universidade']['dociddadosprojeto'], array('aba' => $_REQUEST['aba']) );
		?>
	</td>
	
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
		<?=criarBotoesNavegacao(array('url' => $_SERVER['REQUEST_URI'],'funcao' => 'salvarDadosIES')) ?>
		</td>
	</tr>
</table>
</form>