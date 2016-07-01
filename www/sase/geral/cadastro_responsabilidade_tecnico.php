<?php

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
include_once "../_constantes.php";
$db = new cls_banco();

include APPRAIZ."includes/funcoes_espelhoperfil.php";

$usucpf = $_REQUEST['usucpf'];
$pflcod = (int)$_REQUEST['pflcod'];

/**
 * Regras Server-Side do negócio de responsabilidade
 */
function validaRegrasNegocio( $pflcod ){

	$numeroPermitidoSupervisor = 10;

	switch ( $pflcod ) {
		case PFLCOD_SASE_SUPERVISOR:
			if( count($_POST['usuunidresp']) > $numeroPermitidoSupervisor ){
				echo "<script>alert('Perfil executivo permite somente {$numeroPermitidoSupervisor} técnicos para responsabilidade.');</script>";
				$_POST['usuunidresp'] = array($_POST['usuunidresp'][0]);
			}
			break;
	}

}

/*
 *** INICIO REGISTRO RESPONSABILIDADES ***
*/

if(isset($_REQUEST['enviar'])) {
	// desativa todos os elementos  da responsabilidade dessse usuario
	$sql = "UPDATE
	sase.usuarioresponsabilidade
	set
	rpustatus = 'I'
	where
	usucpf = '$usucpf'
	and pflcod = $pflcod ";

	$db->executar($sql);

    // Valida se o usuário selecionou algum usuário.
    if (is_array($_POST['usuunidresp'])) {
        foreach ($_POST['usuunidresp'] as $usu) {
            // Retorna todos os municípios dos técnicos
            $sql = "SELECT ur.muncod FROM sase.usuarioresponsabilidade ur WHERE ur.usucpf = '{$usu}' AND ur.rpustatus = 'A' AND ur.pflcod = " . PFLCOD_SASE_TECNICO;

            $dados = $db->carregar($sql);

            if ($dados) {
                foreach ($dados as $dado) {
                    $sql = "INSERT INTO sase.usuarioresponsabilidade (muncod, usucpf, rpustatus, rpudata_inc, pflcod, cpftecnico)
				VALUES ('" . $dado['muncod'] . "', '$usucpf', 'A',  now(), '$pflcod', '" . $usu . "')";
                    $db->executar($sql);
                }
            }
        }
    }
	
	atualizarResponsabilidadesSlaves($usucpf,$pflcod);
	
	$db->commit();
	
	?>
	<script>
		
		window.parent.opener.location.reload();self.close();
		
	</script>
		<?
	exit(0);
}

?>

<html>
	<head>
		<META http-equiv="Pragma" content="no-cache">
		<title>Estados e Municípios</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	</head>
	<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">
		<div align=center id="aguarde">
			<img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle">
			<font color=blue size="2">Aguarde! Carregando Dados...</font>
		</div>
		
		<DIV style="OVERFLOW: AUTO; WIDTH: 496px; HEIGHT: 350px; BORDER: 2px SOLID #ECECEC; background-color: White;">
			<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
				<form name="formulario" method="post" action="">
					<input type="hidden" name="enviar" value="">
					<thead>
						<tr>
							<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="4">
								<strong>Selecione o(s) técnicos(s)</strong>
							</td>				
						</tr>
						<tr>
							<td style="text-align: center;"></td>
							<td style="text-align: center;">CPF</td>
							<td style="text-align: center;">Nome</td>
							<td style="text-align: center;">QTD Municípios</td>
						</tr>
					</thead>
					
					<tbody>
						<?php 
						$sql = "SELECT 
									u.usucpf,
									u.usunome,
									(SELECT COUNT(*) FROM sase.usuarioresponsabilidade ur WHERE ur.usucpf = u.usucpf AND ur.pflcod = p.pflcod AND ur.rpustatus = 'A') AS munqtd,
									(SELECT COUNT(*) FROM sase.usuarioresponsabilidade ur WHERE ur.usucpf = '{$usucpf}' AND ur.cpftecnico = u.usucpf AND ur.rpustatus = 'A') AS qtdrespusu
								FROM seguranca.usuario u
								LEFT JOIN seguranca.perfilusuario pu ON (u.usucpf = pu.usucpf)
								LEFT JOIN seguranca.perfil p ON p.pflstatus = 'A' AND p.pflcod = pu.pflcod
								LEFT JOIN seguranca.sistema s ON s.sisid = p.sisid
								WHERE s.sisid = ".SASE_SISID."
								AND p.pflcod = ".PFLCOD_SASE_TECNICO."
								ORDER BY u.usunome";
						//ver($sql);
						$dados = $db->carregar($sql);
						
						foreach ($dados as $dado){
							?>
							<tr bgcolor="#f4f4f4">
								<td align="left" style="border: 0">
									<input type="checkbox" <?php echo $dado['qtdrespusu'] > 0 ? 'checked' : '' ?> name="<?php echo $dado['usucpf'] ?>" id="<?php echo $dado['usucpf'] ?>" onClick="retorna( this, '<?php echo $dado['usucpf'] ?>', '<?php echo $dado['usunome'] ?>' )" />
								</td>
								<td style="text-align: center;"><?php echo formatar_cpf($dado['usucpf']) ?></td>
								<td style="text-align: left;"><?php echo $dado['usunome'] ?></td>
								<td style="text-align: center;"><?php echo $dado['munqtd'] ?></td>
							</tr>
							<?php 
						}
						?>
					</tbody>
				</form>
			</table>
		</DIV>

		<form name="formassocia" style="margin: 0px;" method="POST">
			<input type="hidden" name="usucpf" value="<?=$usucpf?>"> 
			<input type="hidden" name="pflcod" value="<?=$pflcod?>"> 
			<input type="hidden" name="enviar" value=""> 
			<select multiple size="8" name="usuunidresp[]" id="usuunidresp" style="width: 500px;" class="CampoEstilo">
			<?php
				$sql = "SELECT DISTINCT 
							u.usucpf,
							u.usunome
						FROM sase.usuarioresponsabilidade ur
						INNER JOIN seguranca.usuario u ON u.usucpf = ur.cpftecnico
						WHERE ur.usucpf = '{$usucpf}'
						AND ur.rpustatus = 'A'
						ORDER BY u.usunome";
				
				$dados = $db->carregar($sql);
				
				if ($dados){
					foreach ($dados as $dado) {
						print " <option value=\"".$dado['usucpf']."\">".$dado['usunome']."</option>";
					}
				}
			?>
			</select>
		</form>
		<div id="erro"></div>
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#c0c0c0">
				<td align="right" style="padding: 3px;" colspan="3">
					<input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect);enviarFormulario();" id="ok">
				</td>
			</tr>
		</table>
		
	</body>
</html>

<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script language="JavaScript">
	var validadeAcrescimo = true;
	var campoSelect = document.getElementById("usuunidresp");
						
	document.getElementById('aguarde').style.visibility = "hidden";
	document.getElementById('aguarde').style.display  = "none";
	document.getElementById('tabela').style.visibility = "visible";
	document.getElementById('tabela').style.display  = "";

		
	function enviarFormulario(){
		document.formassocia.enviar.value=1;
		document.formassocia.submit();
	}
		
	function retorna( check, usucpf, usunome )
	{
		if ( check.checked ){

			validaRegrasNegocio();

			// põe
			// todo: ajustar para nao avisar novamente aqui
			if( validadeAcrescimo ){
				campoSelect.options[campoSelect.options.length] = new Option( usunome, usucpf, false, false );
			}else{
				check.checked = false;
			}
		}else { 
			// tira
			for( var i = 0; i < campoSelect.options.length; i++ )
			{
				if ( campoSelect.options[i].value == usucpf )
				{
					campoSelect.options[i] = null;
				}
			}	
		}
		sortSelect( campoSelect );
	}
		
	/**
	 * Valida Regras de Negócio Client-Side
	 */
	 function validaRegrasNegocio(){

		<?php if( $pflcod == PFLCOD_SASE_SUPERVISOR ){ ?>
			if( campoSelect.options.length > 9 ){
				if( validadeAcrescimo == true ){
					alert( 'Só são válidos até 10 técnicos para perfil Supervisor.' );
				}
				validadeAcrescimo = false;
			}else{
				validadeAcrescimo = true;
			}

		<?php } else { ?>
			validadeAcrescimo = false;
		<?php } ?>
	}
</script>