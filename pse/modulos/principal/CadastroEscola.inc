<!--	
******************************************************************** 
	Sistema Integrado de Monitoramento do Ministério da Educação
	Módulo: PSE
	Analista: Juvenal
	Programador: Marcelo Santos <celocsantos@gmail.com>
	Finalidade: Cadastro de escolas para o PSE
	Última modificação: 15/10/2009 09:51
*********************************************************************
-->
<?

include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
include APPRAIZ . "includes/classes/dateTime.inc";
VerSessao();
$entid = $_SESSION['pse']['entid'];
$espid = $_SESSION['pse']['espid'];

$sql = "SELECT espparticipapse2009 from pse.escolapse
		WHERE entid=$entid and espano = ".$_SESSION['exercicio'];
$pse = $db->pegaUm($sql);			


if($_POST){
	if(!empty($_REQUEST['pse2009'])){
		$grava=0;
		$check = $_REQUEST['pse2009'];
		$sql = "SELECT espid from pse.escolapse
				WHERE entid=$entid and espano = ".$_SESSION['exercicio'];
		$quant = $db->pegaUm($sql);

		if($quant<>''){
			if($check=='f'){
				$sql = "SELECT count(*)
						FROM pse.pseanoescola pae
						INNER JOIN
							pse.equipesaudefamilia esf ON esf.esfid=pae.esfid
						INNER JOIN
							pse.escolapse esp ON esp.espid=esf.espid
						WHERE pae.paeanoreferencia='2009' AND esp.espid=$espid and esp.espano = ".$_SESSION['exercicio'];
				$quant2 = $db->pegaum($sql);
				if($quant2>0){
					echo "<script>alert('ERRO! \\n Existe(m) dado(s) no PSE lançado(s) com o ano referência 2009!');
								  window.location.href='pse.php?modulo=principal/CadastroEscola&acao=A';
						  </script>";
					exit;
				}
				$grava = 1;
			}
			else
				$grava=1;

			if($grava==1){
				$sql = "UPDATE pse.escolapse set
						espparticipapse2009 = '$check'
						where entid=$entid and espano = ".$_SESSION['exercicio'];
			}

		}
		else {
			$sql = "INSERT INTO pse.escolapse
					(entid,espparticipapse2009, espano)
					values
					($entid,'$check', ".$_SESSION['exercicio'].")";
		}

		$db->executar($sql);
		$db->commit();
		$pse = $check;
		echo "<script>alert('Operação realizada com sucesso!');</script>";
	}
}
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

echo "<br>";
$titulo = "PSE - Programa Saúde na Escola";
monta_titulo( $titulo , '');
$titulo = "Identificação da Escola";
monta_titulo( $titulo, '<img border="0" src="../imagens/obrig.gif"/>Indica Campo Obrigatório.' );

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>

<form method="POST"  name="formulario">
<?php Cabecalho($entid,1); ?>
<tr>
    <td width="100" align='right' class="SubTituloDireita">Participou do PSE/2009?</td>
    <td width="80%" style="background: rgb(238, 238, 238)">
    	<?php
			$checkNao = "";
			$checkSim = "";
			$pse = ($pse=='n'?'':$pse);
			if($pse!="")
				$pse != "t" ? $checkNao = "checked=checked" : $checkSim = "checked=checked";
		?>
    	<input type="radio" value="f" <?=$checkNao?> id="pse2009" name="pse2009" />Não 
    	<input type="radio" value="t"  <?=$checkSim?> id="pse2009" name="pse2009" />Sim
    	<img border="0" src="../imagens/obrig.gif"/>
    </td>
</tr>
<?php if($pflcod == ESCOLA_ESTADUAL || $pflcod == ESCOLA_MUNICIPAL || $pflcod == SUPER_USUARIO){?>
<?php 
	$sql = "SELECT * FROM pse.programacaoexercicio WHERE prsano = '".$_SESSION['exercicio']."'";
	$arr = $db->pegaLinha( $sql );
	$dataAtual = date("Y-m-d");
	$data = new Data();
	if($arr['prsdata_termino']) {
	$resp = $data->diferencaEntreDatas($dataAtual, $arr['prsdata_termino'], 'maiorDataBolean','','');
	if( $resp == NULL ){ ?>
		<tr bgcolor="#cccccc">
			<td style="text-align: center" colspan='2'>
				<input type="button" class="botao" name="btsalvar" value="Salvar" onclick="gravapse2009()">
			</td>
		</tr>	
	<?php } ?>
<?PHP }?>
<?php } else { ?>
		<tr bgcolor="#cccccc">
			<td style="text-align: center" colspan='2'>
				Data de termino não identificada, favor recarregar a página.
			</td>
		</tr>
<?php } ?>
<?=navegacao('cadastroEstadoMunicipioArvore','EquipeSaudeFamilia');?> 
</table>
</form>


<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">

function gravapse2009() {
	document.formulario.submit();

}

</script>

