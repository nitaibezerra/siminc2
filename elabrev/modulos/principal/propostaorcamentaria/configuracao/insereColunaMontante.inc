<?
	/*
		Sistema Simec
		Setor responsável: SPO-MEC
		Desenvolvedor: Equipe Consultores Simec
		Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
		Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com), Renan de Lima Barbosa (e-mail: renandelima@gmail.com)
		Módulo: insereColunaMontante.inc
		
	*/


	$ppoid = $_REQUEST['ppoid'];
	$acao = $_REQUEST['acao'];
	$codigo = $_REQUEST['codigo'];
	$ano = $_REQUEST['ano'];
	
	if ( $acao == 'incluir' ) 
	{
		$sql= "select a.mtrid from elabrev.matriz a where a.mtrid not in (Select distinct mtrid from elabrev.montantematriz where ppoid = ".$ppoid.") order by a.mtrdsc";
	    $RS = $db->carregar($sql);
		
	    foreach ($RS as $linha)
		{
			
					if (isset($_REQUEST['checkInsere'] [$linha['mtrid']]))
					{
						$sql3="insert into elabrev.montantematriz (mtrid,ppoid,mtmvlrlimite,mtmformatofracao,mtmano) values (" .$linha['mtrid']. "," .$ppoid. ",0.00,'I',".$ano.")";
					    $res = $db->executar($sql3);
						
					}

		}
		$db->commit();
	    
 ?>		

		<script>
		alert('Inclusão efetuada com sucesso!');
		opener.location.reload();
		self.close();
		</script>

<?	}
?>

<form method="POST" name="formulario">
<input type="hidden" name="codigo" value="">
<input type="hidden" name="acao" value="">
<input type="hidden" name="ppoid" value="<?=$ppoid?>">
<input type="hidden" name="ano" value="<?=$ano?>">


<?




//teste utilizando a função Monta Lista
$cabecalho = array('Ações','Descrição');
$sql= "select '<input type=\"checkbox\" value=\"'||a.mtrid||'\" name=\"checkInsere['||a.mtrid||']\" >' as acao, a.mtrdsc from elabrev.matriz a where a.mtrano = '" . ( $_SESSION['exercicio'] + 1 ) . "' and a.mtrid not in (Select distinct mtrid from elabrev.montantematriz where ppoid = ".$ppoid.") order by a.mtrdsc";
$Contador = (integer)$db->pegaUm( "select count(a.mtrsigla) from elabrev.matriz a where a.mtrano = '" . ( $_SESSION['exercicio'] + 1 ) . "' and a.mtrid not in (Select distinct mtrid from elabrev.montantematriz where ppoid = ".$ppoid.") ");

$db->monta_lista_simples($sql,$cabecalho,2500,20,'','','');


?>
<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" >
<tr>
<td align="left">
<? if ($Contador > 0) { ?>
<input type="button" name="incluir" value="Incluir" class="botao" onclick="insereColuna();">
<? }?>
<input type="button" name="voltar" value="Fechar" class="botao" onclick="self.close();">
</td>
</tr>
</table>
</form>

<script>
function insereColuna()
{
		document.formulario.acao.value = 'incluir';
		document.formulario.submit();
		return true;
}

</script>