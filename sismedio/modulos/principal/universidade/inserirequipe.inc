<?
include "_funcoes_universidade.php";

if(!$_SESSION['sismedio']['universidade']['uncid']) {
	$al = array("alert"=>"Problemas na navegação, por favor acesse novamente.","javascript"=>"window.opener.location='sismedio.php?modulo=inicio&acao=C';window.close();");
	alertlocation($al);
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

$info = carregarCadastroIESProjeto(array("uncid"=>$_SESSION['sismedio']['universidade']['uncid']));
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sismedio.js"></script>

<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type=text/javascript src=/includes/prototype.js></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>

<script>
function carregaUsuario() {
	divCarregando();
	var usucpf=document.getElementById('iuscpf').value;
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
	
	document.getElementById('iusnome').value=comp.dados.no_pessoa_rf;
	divCarregado();
}

function inserirEquipe() {

	var termos = new Array();
	termos[<?=PFL_COORDENADORADJUNTOIES ?>] = "Atesto que este profissional preenche os requisitos abaixo, previstos na Resolução nº 51, de 11/12/2013:\n\nI - é professor efetivo de instituição de ensino superior;\nII - tem experiência na área de formação de profissionais da educação básica; e\nIII - possui titulação de mestrado ou doutorado.";
	termos[<?=PFL_SUPERVISORIES ?>] = "Atesto que este profissional preenche os requisitos abaixo, previstos na Resolução nº 51, de 11/12/2013:\n\nI - tem licenciatura ou complementação pedagógica;\nII - é professor ou coordenador pedagógico efetivo da rede de ensino ou professor de Instituição de Ensino Superior ou está cursando mestrado e/ou doutorado na área educacional;\nIII - possui titulação de especialização, mestrado ou doutorado; e\nIV - tem disponibilidade de 20h semanais para dedicar-se à função.";
	termos[<?=PFL_FORMADORIES ?>] = "Atesto que este profissional preenche os requisitos abaixo, previstos na Resolução nº 51, de 11/12/2013:\n\nI - é professor de instituição de ensino superior;\nII - tem experiência na educação básica;\nIII - é formado em Pedagogia ou Licenciatura; e\nVI - possui mestrado ou doutorado ou está cursando pós-graduação strictu senso na área de Educação ou áreas afins.";
	termos[<?=PFL_FORMADORREGIONAL ?>] = "Atesto que este profissional preenche os requisitos abaixo, previstos no Artigo 10 da Resolução nº 51, de 11/12/2013:\n\nI - é profissional efetivo da rede pública de ensino;\nII - tem experiência como professor ou coordenador pedagógico do Ensino Médio ou atuou em formação continuada de profissionais da educação básica durante, pelo menos, dois anos;\nIII - tem titulação de especialização, mestrado ou doutorado ou está cursando pós-graduação na área de Educação; e\nVI - tem disponibilidade para dedicar-se ao curso de formação, aos encontros com os formadores das IES e ao trabalho de formação na região com orientadores de estudo, correspondente a 20 horas semanais.";
	
	if(jQuery('#pflcod').val()=='') {
		alert('Função em branco');
		return false;
	}
	
	jQuery('#iuscpf').val(mascaraglobal('###.###.###-##',jQuery('#iuscpf').val()));
	
	if(jQuery('#iuscpf').val()=='') {
		alert('CPF em branco');
		return false;
	}
	
	if(!validar_cpf(jQuery('#iuscpf').val())) {
		alert('CPF inválido');
		return false;
	}
	
	if(jQuery('#iusnome').val()=='') {
		alert('Nome em branco');
		return false;
	}
	
	if(jQuery('#iusemailprincipal').val()=='') {
		alert('Email Principal em branco');
		return false;
	}
	
    if(!validaEmail(jQuery('#iusemailprincipal').val())) {
    	alert('Email Principal inválido');
    	return false;
    }
    
	if(jQuery('#itedddtel_T').val()=='') {
		alert('DDD Trabalho em branco');
		return false;
	}
	
	if(jQuery('#itedddtel_T').val().length!=2) {
		alert('DDD Trabalho deve possuir 2 dígitos');
		return false;
	}
	
	if(jQuery('#itenumtel_T').val()=='') {
		alert('Telefone Trabalho em branco');
		return false;
	}
	
	if(jQuery('#itenumtel_T').val().length!=9) {
		alert('Telefone Trabalho deve possuir 8 dígitos');
		return false;
	}
	
	if(jQuery('#foeid').val()=='') {
		alert('Escolaridade em branco');
		return false;
	}
	
	// regra fixa
	if(jQuery('#pflcod').val()=='<?=PFL_COORDENADORADJUNTOIES ?>') {
		if(jQuery('#foeid').val()!='<?=FOE_ESPECIALIZACAO ?>' && jQuery('#foeid').val()!='<?=FOE_MESTRADO ?>' && jQuery('#foeid').val()!='<?=FOE_DOUTORADO ?>') {
			alert('Escolaridade mínima para Coordenador adjunto: especialização, mestrado ou doutorado');
			return false;		
		}
	}
	// regra fixa
	if(jQuery('#pflcod').val()=='<?=PFL_SUPERVISORIES ?>') {
		if(jQuery('#foeid').val()!='<?=FOE_ESPECIALIZACAO ?>' && jQuery('#foeid').val()!='<?=FOE_MESTRADO ?>' && jQuery('#foeid').val()!='<?=FOE_DOUTORADO ?>') {
			alert('Escolaridade mínima para Supervisor da IES: especialização, mestrado ou doutorado');
			return false;		
		}
	}
	
	if(jQuery('#tpeatuacaoinicio_mes').val()=='') {
		alert('Inicío - Mês - Período de atuação em branco');
		return false;
	}

	if(jQuery('#tpeatuacaoinicio_ano').val()=='') {
		alert('Inicío - Ano - Período de atuação em branco');
		return false;
	}
	
	// regra fixa - solicitada
	if(jQuery('#tpeatuacaoinicio_ano').val()=='2012') {
		if(parseInt(jQuery('#tpeatuacaoinicio_mes').val())< 10) {
			alert('Não aceita data inicial anterior a Outubro/2012');
			return false;
		}
	}
	
	if(jQuery('#tpeatuacaofim_mes').val()=='') {
		alert('Fim - Mês - Período de atuação em branco');
		return false;
	}

	if(jQuery('#tpeatuacaofim_ano').val()=='') {
		alert('Fim - Ano - Período de atuação em branco');
		return false;
	}
	
	dt1 = parseInt(jQuery('#tpeatuacaoinicio_ano').val()+jQuery('#tpeatuacaoinicio_mes').val());
	dt2 = parseInt(jQuery('#tpeatuacaofim_ano').val()+jQuery('#tpeatuacaofim_mes').val());
	if(dt2<dt1) {
		alert('Período de atuação início não pode ser maior que o término');
		return false;
	}
	
	if('<?=$info['universidade']['uncdatafimprojeto'] ?>'=='') {
		alert('Preencha a Vigência do projeto na aba Dados gerais do projeto');
		return false;
	}
	
	if(dt2 > '<?=substr(str_replace("-","",$info['universidade']['uncdatafimprojeto']),0,6) ?>') {
		alert('O Término do período de atuação não pode ser maior que Término da Vigência do projeto');
		return false;
	}


	/*
	if(jQuery('#pflcod').val()=='<?=PFL_FORMADORIES ?>') {
		if(jQuery('#tpejustificativaformadories').val()=='') {
		<? if(!validarNumeroFormadorIES(array("uncid" => $_SESSION['sismedio']['universidade']['uncid']))) : ?>
		jQuery("#modalEquipe").dialog({
		                        draggable:true,
		                        resizable:true,
		                        width: 400,
		                        height: 300,
		                        modal: true,
		                     	close: function(){} 
		                    });
		return false;
		<? endif; ?>
		}
	}
	*/
	
	var conf = confirm(termos[jQuery('#pflcod').val()]);
	
	if(!conf) {
		jQuery('#tpejustificativaformadoriesprev').val('');
		return false;
	}

	
    divCarregando();
    
	document.getElementById('formulario').submit();

}


function tipoSupervisor(tipo) {

	if(tipo=='I') {
		jQuery('#sp_iustiposupervisories').css('display','');
	} else {
		jQuery('#sp_iustiposupervisories').css('display','none');
	}
	
}

function verificarFuncao(pflcod) {

	if(pflcod=='<?=PFL_SUPERVISORIES ?>') {
		jQuery('#tr_tiposupervisor').css('display','');
	} else {
		jQuery('#tr_tiposupervisor').css('display','none');
	}
	
}


</script>
<?
if($_REQUEST['iusd']) {
	$sql = "SELECT tp.tpeid, t.iteid, i.iusd, tp.pflcod, i.iuscpf, i.iusnome, i.iusemailprincipal, t.itedddtel, t.itenumtel, i.foeid, i.iussupervisoranalista, i.iustiposupervisor, i.iustiposupervisories,
				   to_char(tpeatuacaoinicio,'MM') as tpeatuacaoinicio_mes,
				   to_char(tpeatuacaoinicio,'YYYY') as tpeatuacaoinicio_ano,
				   to_char(tpeatuacaofim,'MM') as tpeatuacaofim_mes,
				   to_char(tpeatuacaofim,'YYYY') as tpeatuacaofim_ano
			FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.identificacaotelefone t ON t.iusd = i.iusd AND t.itetipo='T' 
			INNER JOIN sismedio.tipoperfil tp ON tp.iusd = i.iusd 
			WHERE i.iusd='".$_REQUEST['iusd']."'";
	
	$identificacaousuario = $db->pegaLinha($sql);
	
	if($identificacaousuario) extract($identificacaousuario);
	
}
?>
<div id="modalEquipe" style="display:none;">
<script language="JavaScript" src="../includes/funcoes.js"></script>

<script>
function inserirJustificativa() {
	if(jQuery('#tpejustificativaformadoriesprev').val()=='') {
		alert('Justificativa em branco');
		return false;
	}
	
	jQuery('#tpejustificativaformadories').val(jQuery('#tpejustificativaformadoriesprev').val());
	inserirEquipe();
}
</script>
<form method="post" id="justificativa">
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloCentro" colspan="2">Justificativa</td>
</tr>
<tr>
	<td>Vagas disponíveis para formadores já foram preenchidas. Para inserir novos formadores, é necessário apresentar uma justificativa:</td>
</tr>
<tr>
	<td><?=campo_textarea( 'tpejustificativaformadoriesprev', 'S', 'S', '', '50', '4', '200'); ?></td>
</tr>
<tr>	
	<td class="SubTituloCentro" colspan="2"><input type="button" value="Inserir" onclick="inserirJustificativa();"></td>
</tr>
</table>
</form>
</div>

<form method="post" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="inserirEquipeRecursosHumanos">
<input type="hidden" name="iusd" value="<?=$iusd ?>">
<input type="hidden" name="uncid" value="<?=$_SESSION['sismedio']['universidade']['uncid'] ?>">
<input type="hidden" name="tpejustificativaformadories" id="tpejustificativaformadories" value="">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Inserir Equipe</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Função:</td>
	<td><?
	$sql = "SELECT pflcod as codigo, pfldsc as descricao FROM seguranca.perfil WHERE pflcod IN('".PFL_FORMADORIES."','".PFL_FORMADORREGIONAL."','".PFL_SUPERVISORIES."','".PFL_COORDENADORADJUNTOIES."') ORDER BY pflcod";
	$db->monta_combo('pflcod', $sql, 'S', 'Selecione', 'verificarFuncao', '', '', '', 'S', 'pflcod', ''); 
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">CPF</td>
	<td><?=campo_texto('iuscpf', "S", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="iuscpf"', '', mascaraglobal($iuscpf,"###.###.###-##"), 'if(this.value!=\'\'){carregaUsuario();}'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Nome</td>
	<td><?=campo_texto('iusnome', "S", "N", "Nome", 67, 150, "", "", '', '', 0, 'id="iusnome"', ''); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Email</td>
	<td><?=campo_texto('iusemailprincipal', "S", "S", "Principal", 67, 60, "", "", '', '', 0, 'id="iusemailprincipal"'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Trabalho</td>
	<td><?=campo_texto('itedddtel[T]', "N", "S", "DDD", 3, 2, "##", "", '', '', 0, 'id="itedddtel_T"', '', $itedddtel ); ?> <?=campo_texto('itenumtel[T]', "S", "S", "Telefone", 10, 9, "####-####", "", '', '', 0, 'id="itenumtel_T"', '', $itenumtel ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Escolaridade</td>
	<td><?
	$sql = "SELECT foeid as codigo, foeid||' - '||foedesc as descricao 
			FROM sismedio.formacaoescolaridade 
			WHERE foestatus='A' AND foeid IN('".FOE_SUPERIOR_COMPLETO_PEDAGOGIA."',
											 '".FOE_SUPERIOR_COMPLETO_LICENCIATURA."',
											 '".FOE_SUPERIOR_COMPLETO_OUTRO."',
											 '".FOE_ESPECIALIZACAO."',
											 '".FOE_MESTRADO."',
											 '".FOE_DOUTORADO."') 
			ORDER BY foeid";
	
	$db->monta_combo('foeid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'foeid', '');
	?></td>
</tr>
<tr style="<?=(($pflcod==PFL_SUPERVISORIES)?"":"display: none;")?>" id="tr_tiposupervisor">
	<td class="SubTituloDireita">Tipo de supervisor</td>
	<td><?
	
	$arr_tipo_s[] = array('codigo'=>'S','descricao'=>'Secretaria Estadual');
	$arr_tipo_s[] = array('codigo'=>'I','descricao'=>'IES');
	
	
	$db->monta_combo('iustiposupervisor', $arr_tipo_s, 'S', 'Selecione', 'tipoSupervisor', '', '', '', 'S', 'iustiposupervisor', '');
	?>
	<span style="<?=(($iustiposupervisor=='I')?"":"display:none") ?>" id="sp_iustiposupervisories"><font style=font-size:xx-small;>Qual IES?</font> <?=campo_texto('iustiposupervisories', "N", "S", "Sigla", 30, 100, "", "", '', '', 0, 'id="iustiposupervisories"', '', $iustiposupervisories ); ?></span>
	<br>
	<font style=font-size:xx-small;><input type="checkbox" name="iussupervisoranalista" value="TRUE" <?=(($iussupervisoranalista=='t')?"checked":"")?>> Supervisor analista
	
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Período de atuação</td>
	<td>
	Inicío: 
	<?
	$sql = "SELECT mescod as codigo, mesdsc as descricao FROM public.meses";
	$db->monta_combo('tpeatuacaoinicio_mes', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'tpeatuacaoinicio_mes', ''); 
	echo "&nbsp;";
	$sql = "SELECT ano as codigo, ano as descricao FROM public.anos WHERE ano>='2013'";
	$db->monta_combo('tpeatuacaoinicio_ano', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'tpeatuacaoinicio_ano', ''); 
	?> 
	Término: 
	<? 
	$sql = "SELECT mescod as codigo, mesdsc as descricao FROM public.meses";
	$db->monta_combo('tpeatuacaofim_mes', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'tpeatuacaofim_mes', ''); 
	echo "&nbsp;";
	$sql = "SELECT ano as codigo, ano as descricao FROM public.anos WHERE ano>='2012'";
	$db->monta_combo('tpeatuacaofim_ano', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'tpeatuacaofim_ano', '');
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" value="Salvar" onclick="inserirEquipe();"> <input type="button" value="Fechar" onclick="window.close();"></td>
</tr>
</table>
</form>
