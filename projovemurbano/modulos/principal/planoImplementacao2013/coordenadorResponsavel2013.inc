<?php
$sql = "SELECT * FROM projovemurbano.coordenadorresponsavel WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'";
$coordenadorresponsavel = $db->pegaLinha($sql);

if($coordenadorresponsavel) {
	$requisicao = "atualizarCoordenadorResponsavelPorAno";
	extract($coordenadorresponsavel);
} else {
	$requisicao = "inserirCoordenadorResponsavelPorAno";
}

$habilita = "N";
$disabilita = 'S';
$perfil = pegaPerfilGeral();
if( in_array(PFL_SECRETARIO_MUNICIPAL, $perfil) || in_array(PFL_SECRETARIO_ESTADUAL, $perfil) || 
	in_array(PFL_EQUIPE_MEC, $perfil) || in_array(PFL_ADMINISTRADOR, $perfil) || $db->testa_superuser()  ){
    $habilita = 'S';
	$disabilita = '';
}

?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script>
function salvarCoordenadorResponsavel() {

	var corsecretario = jQuery("[name^='corsecretario']:checked");
	if(corsecretario.length==0) {
		alert('Marque se é o próprio secretário');
		return false;
	}
	
	if(jQuery('#corcpf').val()=="") {
		alert('Digite um cpf');
		return false;
	}
	
	if(jQuery('#corcpf').val()=="") {
		alert('Digite um cpf');
		return false;
	}
	
	if(!validar_cpf(jQuery('#corcpf').val())) {
		alert('CPF incorreto');
		return false;
	}
	
	if(jQuery('#cornome').val()=="") {
		alert('Digite um nome');
		return false;
	}
	
	jQuery('#form').submit();

}

function proprioSecretario(obj) {
	if(obj.value=="sim") {
		jQuery('#corcpf').attr("readonly",true);
		jQuery('#corcpf').attr("class","disabled");
		jQuery('#cornome').attr("readonly",true);
		jQuery('#cornome').attr("class","disabled");
		jQuery('#corcpf').val('<?=mascaraglobal($_SESSION['usucpf'],'###.###.###-##') ?>');
		jQuery('#cornome').val('<?=$_SESSION['usunome'] ?>');
	}
	if(obj.value=="nao") {
		jQuery('#corcpf').attr("readonly",false);
		jQuery('#corcpf').attr("class","normal");
		jQuery('#cornome').attr("readonly",false);
		jQuery('#cornome').attr("class","normal");
		jQuery('#corcpf').val('');
		jQuery('#cornome').val('');

	}
}

/* Função para subustituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}


function buscarNome(usucpf) {
	divCarregando();
	usucpf = replaceAll(replaceAll(usucpf,'.',''),'-','');
	var comp = new dCPF();
	comp.buscarDados(usucpf);
	if(!comp.dados.no_pessoa_rf){
		divCarregado();
		alert('CPF Inválido');
		return false;
	}
	document.getElementById('cornome').value=comp.dados.no_pessoa_rf;
	divCarregado();
}

//Faz a verificação do perfil e desabilita o radio button
jQuery(document).ready(function(){
	<?php if($disabilita == 'S'){?>
		Disabilitado(this);
	<?php } ?>
	function Disabilitado() {
	    jQuery("input[name='corsecretario']").each(function(i) {
	        jQuery(this).attr('disabled', 'disabled');
		});
	}	
});

</script>
<form id="form" name="form" method="POST">
<input type="hidden" name="requisicao" value="<?=$requisicao; ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">Orientações:</td>
		<td>
			<font color=blue>
				<p>Somente o Secretário de Educação ou o Coordenador Geral indicado 
				poderá desenvolver o Plano de Implementação que deverá ser validado 
				apenas pelo Secretário de Educação.</p>
				<p>
				A assinatura do Secretário de Educação será a única aceita na versão final 
				do Plano de Implementação e deverá ser impressa para envio à DPEJUV/
				SECADI/MEC.</p>
			</font>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">É o próprio secretário?</td>
		<td>
			<input type="radio" name="corsecretario" value="sim" onclick="proprioSecretario(this);" <?=(($corsecretario=="t")?"checked":"") ?> > Sim 
			<input type="radio" name="corsecretario" value="nao" onclick="proprioSecretario(this);" <?=(($corsecretario=="f")?"checked":"") ?> > Não
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">CPF:</td>
		<td><? echo campo_texto('corcpf', 'S', (($corsecretario=="t" || $habilita=='N')?'N':'S'), 'CPF', 16, 14, "###.###.###-##", "", '', '', 0, 'id="corcpf"', '', mascaraglobal($corcpf,"###.###.###-##"), 'buscarNome(this.value);' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome:</td>
		<td><? echo campo_texto('cornome', 'S', (($corsecretario=="t" || $habilita=='N')?'N':'S'), 'Nome', 50, 200, "", "", '', '', 0, 'id="cornome"', '', $cornome ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">
			<?php if( $habilita == 'S' ){ ?> 
				<input type="button" name="salvar" value="Salvar" onclick="salvarCoordenadorResponsavel();"> 
			<?php } ?>
		</td>
	</tr>
</table>
</form>
<? registarUltimoAcesso(); ?>
