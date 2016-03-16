<?php

if( $_REQUEST["requisicao"] == "salvaDados" ){
	
	$emi->cadastraPapEscola( $_REQUEST );
	
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
include_once APPRAIZ . "includes/workflow.php";
print "<br/>";

$db->cria_aba($abacod_tela,$url,$parametros);

if ( $_REQUEST["acao"] == "A" ){

	if ( $_REQUEST["emeid"] || $_SESSION["emi"]["emeidEscola"] ){
	
		$_SESSION["emi"]["emeidEscola"] = !empty($_REQUEST["emeid"]) ? $_REQUEST["emeid"] : $_SESSION["emi"]["emeidEscola"];
		$emeid = $_SESSION["emi"]["emeidEscola"];
		
		if ( !emiVerificaEntidade( $emeid ) ){
			
			unset( $_SESSION["emi"]["emeidEscola"] );
			
			print "<script>
					  alert('A escola selecionada não existe ou está em branco!');
					  history.back(-1);
				  </script>";
			die;
		}
		
	}
	
	monta_titulo("PAP {$_SESSION['exercicio']} da Escola", "");
	
	if( $_SESSION["emi"]["emeidEscola"] ){
		$emi->montaCabecalho( $_SESSION["emi"]["emeidEscola"], "escola" );
	}
	
	$tipo = "escola";
	
}else if( $_REQUEST["acao"] == "C" ){

	if ( $_REQUEST["emeid"] || $_SESSION["emi"]["emeidPai"] ){
	
		$_SESSION["emi"]["emeidPai"] = !empty($_REQUEST["emeid"]) ? $_REQUEST["emeid"] : $_SESSION["emi"]["emeidPai"];
		$emeid = $_SESSION["emi"]["emeidPai"];
		
		if ( !emiVerificaEntidade( $emeid ) ){
			
			unset( $_SESSION["emi"]["emeidPai"] );
			
			print "<script>
					  alert('A secretaria selecionada não existe ou está em branco!');
					  history.back(-1);
				  </script>";
			die;
		}
		
	}
	
	monta_titulo("PAP {$_SESSION['exercicio']} da Secretaria", "");
	
	if( $_SESSION["emi"]["emeidPai"] ){
		$emi->montaCabecalho( $_SESSION["emi"]["emeidPai"], "secretaria" );
	}
	
	$tipo = "secretaria";
	
}

$emeid = !$_REQUEST['emeid'] ? $_SESSION["emi"]["emeidEscola"] : $_REQUEST['emeid'];

$_SESSION["emi"]["emiid"] = $emeid;

if($emeid){
	$docid = emiCriarDocumento( $emeid );
}

?>

<script type="text/javascript" src="/includes/prototype.js"></script>

<form method="post" id="emiSelecionaEscolas" action="">
	<input type="hidden" name="requisicao" value="salvaDados"/>
	<input type="hidden" id="comid" name="comid" value=""/>
	<input type="hidden" id="emeid" name="emeid" value="<?=$emeid?>"/>
	<table class="tabela" height="100%" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>
			<td valign="top" >
				<table class="tabela" bgcolor="#f5f5f5" height="100%" cellspacing="2" cellpadding="4" align="center" style="border-collapse: collapse;">
					<tr>
						<td class="subtitulocentro" width="10%">Dimensões / PAR</td>
						<td class="subtitulocentro" width="25%">Linha de Ação</td>
						<td class="subtitulocentro" width="25%">Componentes</td>
						<td class="subtitulocentro" width="40%">Preenchimento da Secretaria de Estado de Educação</td>
					</tr>
					
					<?php
					if(!$emeid) {
						die("<script>
								alert('Problemas com variáveis. Refaça o procedimento.');
								window.location='emi.php?modulo=inicio&acao=C';
							  </script>");
					}
					
					emiMontaPapEscola( $emeid, $tipo ); 
					
					?>
					
				</table>
			</td>
			<td valign="top" >
				<?php if($_REQUEST['emeid'] != $_SESSION['emi']['emeidPai']): ?>

					<?php wf_desenhaBarraNavegacao( $docid , array("emeidPai" => $emeid) ); ?>
				<?php endif; ?>
			</td>
		</tr>
	</table>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr bgcolor="#C0C0C0">
			<td colspan="5">
				<input type="button" value="Voltar" onclick="history.back(-1);" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>