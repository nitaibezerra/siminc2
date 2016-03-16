<style>.link{cursor:pointer}</style>
<script>
function carregarUnidade(hsuid)
{
	window.location.href='academico.php?modulo=principal/maisMedicosUnidade&acao=A&hsuid='+hsuid;
}
</script>
<?php
include_once(APPRAIZ."academico/classes/MaisMedicos.class.inc");
include_once(APPRAIZ."academico/classes/HospitalUnidade.class.inc");
include_once(APPRAIZ."academico/classes/MaisMedicosResponsabilidade.class.inc");

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

monta_titulo( "Lista de Unidades", "Selecione a Unidade para visualizar o Termo de Pré-Adesão.");

$arrPerfil = pegaPerfilGeral();
if(!in_array(PERFIL_MAIS_MEDICOS_UNIDADE,$arrPerfil)){
	$erro_js = "Você não possui perfil para visualizar esta tela.";
}else{
	$sql = "select 
				'<img src=\"../imagens/consultar.gif\" class=\"link\"  onclick=\"carregarUnidade(' || h.hsuid || ')\"  />' as acao,
				h.hsudsc 
			from 
				academico.usuarioresponsabilidade ur
			inner join
				academico.hospitalunidade h ON h.hsuid = ur.hsuid 
			where 
				ur.usucpf = '{$_SESSION['usucpf']}' 
			and 
				ur.pflcod = ".PERFIL_MAIS_MEDICOS_UNIDADE." 
			and 
				ur.rpustatus = 'A'
			and
				h.hsustatus = 'A'";
	$arrCab = array("Ação","Unidade");
	$db->monta_lista($sql,$arrCab,100,10,"N","center","N");
	
}