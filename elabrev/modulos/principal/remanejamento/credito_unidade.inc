<?php

//$edicao_ativa = cre_edicao_ativa();

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
//$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Unidade', 'Atualização dos dados da Unidade.' );

/*** Verifica se perfil do usuário é de Super usuário, CGO / Coordenador de Orçamento ou CGO / Equipe Orçamentária ***/
$pflcods = array(SUPER_USUARIO,CGO_COORDENADOR_ORCAMENTO,CGO_EQUIPE_ORCAMENTARIA);

if( possuiPerfil( $pflcods ) )
{
	?>
	<script>
		location.href="elabrev.php?modulo=principal/remanejamento/credito&acao=A";
	</script>
	<? 
	die;
}

//if ( $edicao_ativa && isset( $_REQUEST['form'] ) ) 
if ( $_POST )
{

	if($_POST['unicod']){
		
		for($i=0; $i<count($_POST['unicod']); $i++){
			$sql =	" update public.unidade 
					 set 
						 uniddd = '" . ( $_POST['uniddd'][$i] ) . "',
						 unitelefone = '" . ( str_replace("-","",$_POST['unitelefone'][$i]) ) . "',
						 uniemail = '" . ( $_POST['uniemail'][$i] ) . "',
						 unidataatualiza = now()  
					 where 
						 unicod = '" . $_POST['unicod'][$i] . "'
					 ";
			$db->executar( $sql );
		}
		$db->commit();
		
		echo "<script>alert('Unidade(s) atualizada(s).');</script>";
	}
		
	?>
	<script>
		location.href="elabrev.php?modulo=principal/remanejamento/credito&acao=A";
	</script>
	<? 
	die;
	
}



$sql = "select distinct u.unicod, u.unidsc, u.uniddd, u.unitelefone, u.uniemail 
		from public.unidade u 
		inner join elabrev.usuarioresponsabilidade ur ON ur.unicod = u.unicod
		where ur.rpustatus='A' and ur.usucpf = '" . $_SESSION['usucpf'] . "' order by 1";

$dados = $db->carregar( $sql );
/*
$unicod = $dados['unicod'];
$unidsc = $dados['unidsc'];
$uniddd = $dados['uniddd'];
if($dados['unitelefone']) $unitelefone = formata_fone_fax($dados['unitelefone']);
$uniemail = $dados['uniemail'];
*/
?>

<form action="" method="post" name="formulario">
	
	<?
	if($dados){
		$i=0;
		foreach($dados as $d){
		?>
			<table class="tabela" width="90%" cellSpacing="1"  cellPadding="3" bgcolor="#f5f5f5" align="center" style="border-bottom: 0px;">
				<tr>
					<td width="20%" align="right" class="SubTituloDireita">Unidade:</td>
					<td width="80%">
						<input type="hidden" name="unicod[]" value="<?=$dados[$i]['unicod']?>">
						<b><?= $dados[$i]['unicod'] ?></b> - <?= $dados[$i]['unidsc'] ?>
					</td>
				</tr>
				<tr>
					<td width="20%" align="right" class="SubTituloDireita">Telefone:<br>Ex:(61) 9999-9999</td>
					<td width="80%">
						(<?= campo_texto( 'uniddd[]', 'N', 'S', 'DDD', 2, 2, '##', 'N', '','','','','', $dados[$i]['uniddd'] ); ?>)
						<?= campo_texto( 'unitelefone[]', 'N', 'S', 'Telefone', 11, 9, '####-####', 'N', '','','','','', ($dados[$i]['unitelefone'] ? formata_fone_fax($dados[$i]['unitelefone']) : '' ) ); ?>
					</td>
				</tr>
				<tr>
					<td width="20%" align="right" class="SubTituloDireita">E-mail:</td>
					<td width="80%">
						<?= campo_texto( 'uniemail[]', 'N', 'S', 'E-mail', 80, 150, '', 'N', '','','','','', $dados[$i]['uniemail'] ); ?>
					</td>
				</tr>
			</table>
		<?
		$i++;
		}
	}
	?>
	
	<table class="tabela" width="90%" cellSpacing="1"  cellPadding="3" bgcolor="#f5f5f5" align="center" style="border-bottom: 0px;">
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				&nbsp;
			</td>
			<td width="80%">
				<input type="submit" name="Salvar" value="Salvar" />
			</td>
		</tr>
	</table>
	
</form>






