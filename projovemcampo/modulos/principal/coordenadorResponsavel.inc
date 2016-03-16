<?php
$sql = "SELECT 	
			s.secoordcpf,
			so.secocpf,
			u.usunome
		FROM 
					projovemcampo.secretaria s
		INNER JOIN	projovemcampo.secretario so ON so.secoid = s.secoid
		INNER JOIN	projovemcampo.adesaoprojovemcampo a ON a.secaid = s.secaid
		INNER JOIN  seguranca.usuario u ON u.usucpf = s.secoordcpf
		WHERE 
			apcid='".$_SESSION['projovemcampo']['apcid']."'";

$coordenadorresponsavel = $db->pegaLinha($sql);
// ver($_SESSION['projovemcampo']);
$requisicao = "atualizarCoordenadorResponsavel";
if($coordenadorresponsavel){
	extract($coordenadorresponsavel);
// 	if($secocpf == $secoordcpf){
// 		$corsecretario="t";
// 	}else{
// 		$corsecretario="f";
// 	}
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
// 	if(corsecretario.length==0) {
// 		alert('Marque se é o próprio secretário');
// 		return false;
// 	}
	
	if(jQuery('#secoordcpf').val()=="") {
		alert('Digite um cpf');
		return false;
	}
	
	if(jQuery('#secoordcpf').val()=="") {
		alert('Digite um cpf');
		return false;
	}
	
	if(!validar_cpf(jQuery('#secoordcpf').val())) {
		alert('CPF incorreto');
		return false;
	}
	
	if(jQuery('#usunome').val()=="") {
		alert('Digite um nome');
		return false;
	}
	
	jQuery('#form').submit();

}
// function proprioSecretario(obj) {
// 	if(obj.value=="sim") {
// 		jQuery('#secoordcpf').attr("readonly",true);
// 		jQuery('#secoordcpf').attr("class","disabled");
// 		jQuery('#usunome').attr("readonly",true);
// 		jQuery('#usunome').attr("class","disabled");
//		jQuery('#secoordcpf').val('<?/*=mascaraglobal($_SESSION['usucpf'],'###.###.###-##') */?>');
//		jQuery('#usunome').val('<?/*=$_SESSION['usunome'] */?>');
// 	}
// 	if(obj.value=="nao") {
// 		jQuery('#secoordcpf').attr("readonly",false);
// 		jQuery('#secoordcpf').attr("class","normal");
// 		jQuery('#usunome').attr("readonly",false);
// 		jQuery('#usunome').attr("class","normal");
// 		jQuery('#secoordcpf').val('');
// 		jQuery('#usunome').val('');

// 	}
// }

/* Função para subustituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}


function buscarNome(secoordcpf) {
// 	divCarregando();
	secoordcpf = replaceAll(replaceAll(secoordcpf,'.',''),'-','');
	var comp = new dCPF();
	comp.buscarDados(secoordcpf);
// 	if(!comp.dados.no_pessoa_rf){
// 		divCarregado();
// 		alert('CPF Inválido');
// 		return false;
// 	}
	document.getElementById('usunome').value=comp.dados.no_pessoa_rf;
	divCarregado();
}

//Faz a verificação do perfil e desabilita o radio button
jQuery(document).ready(function(){
	jQuery('#secoordcpf').blur(function(){
		buscarNome(this.value);
	});
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
<?$secoordcpf = $secoordcpf;?>
<form id="form" name="form" method="POST">
<input type="hidden" name="requisicao" value="<?=$requisicao; ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<!-- 	<tr> -->
<!-- 		<td class="SubTituloDireita">Orientações:</td> -->
<!-- 		<td> -->
<!-- 		<font color=blue> -->
<!-- 		<p>Somente o secretário ou o coordenador indicado poderá desenvolver o plano de implementação que deverá ser validado apenas pelo secretário.</p> -->
<!-- 			<p>A assinatura do secretário será a única  aceita na versão final do plano de implementação e  deverá ser impressa para envio a SECADI/MEC.</p> -->
<!-- 			</font> -->
<!-- 		</td> -->
<!-- 	</tr> -->
<!-- 	<tr> -->
<!-- 		<td class="SubTituloDireita">É o próprio secretário?</td> -->
<!-- 		<td> -->
<!-- 			<input type="radio" name="corsecretario" value="sim" onclick="proprioSecretario(this);" <?/*=(($corsecretario=="t")?"checked":"") */?> > Sim -->
<!-- 			<input type="radio" name="corsecretario" value="nao" onclick="proprioSecretario(this);" <?/*=(($corsecretario=="f")?"checked":"") */?> > Não -->
<!-- 		</td> -->
<!-- 	</tr> -->
	<tr>
		<td class="SubTituloDireita">CPF:</td>
		<td><? echo campo_texto('secoordcpf','S', (($corsecretario=="t" || $habilita=='N')?'N':'S'), 'CPF', 16, 14, "###.###.###-##", "", '', '', 0, 'id="secoordcpf"','',  mascaraglobal($secoordcpf,"###.###.###-##")); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome:</td>
		<td><? echo campo_texto('usunome', 'S', (($corsecretario=="t" || $habilita=='N')?'N':'S'), 'Nome', 50, 200, "", "", '', '', 0, 'id="usunome"', '', $usunome ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">
		<!-- 	<input type="button" name="anterior" value="Anterior" onclick="window.location='projovemcampo.php?modulo=principal/sugestaoAmpliacao&acao=A';"> -->
			<?php if( $habilita == 'S' ){ ?> 
				<input type="button" name="salvar" value="Salvar" onclick="salvarCoordenadorResponsavel();"> 
			<?php } ?>
		<!-- 	<input type="button" name="proximo" value="Próximo" onclick="window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=metaMatriculaInicioAula';"> -->
		</td>
	</tr>
</table>
</form>
<?registarUltimoAcesso();?>