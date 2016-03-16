<?php 
require_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/workflow.php";

if ( !$db )
{
    $db = new cls_banco();
}

$docid = (integer) $_REQUEST['docid'];

$acaoestadodoc = $db->pegaLinha("
    SELECT 
        aed.aedid,
        aed.esdiddestino AS esdid,
        esd.esddsc 
    FROM 
        workflow.acaoestadodoc aed
        INNER JOIN workflow.estadodocumento esd ON esd.esdid = aed.esdiddestino
    WHERE 
        aed.esdidorigem = (
            SELECT 
                esdid 
            FROM 
                workflow.documento 
            WHERE 
                docid = {$docid}
        );
");

$esdid = $acaoestadodoc['esdid'];
$aedid = $acaoestadodoc['aedid'];

$verificacao = stripcslashes( (string) $_REQUEST['verificacao'] );
$documento = wf_pegarDocumento( $docid );
$acao = wf_pegarAcao($documento['esdid'], $esdid);

if (!$documento || !$acao)
{
    exit("
        <script type=\"text/javascript\">
    		alert('Documento ou ação inválida.');
    		window.close();
        </script>
    ");
}

if($_POST) {
    if(!empty($_FILES['arquivo']['name'])) {
    
        require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    
        $campos	= array("dmdid" => $_REQUEST['dmdid']);
    
        $file = new FilesSimec("demandaanexo", $campos, 'par');
    
        if($_FILES["arquivo"]){
            $arquivoSalvo = $file->setUpload($_FILES["arquivo"]['name']);
            if( $arquivoSalvo ){
                if(wf_alterarEstado($docid, $aedid, $_POST['cmddsc'], array())) {
                    exit("
                        <script type=\"text/javascript\">
                    		alert('Estado alterado com sucesso!');
                    		window.opener.location.reload();
                        </script>
                    ");
                }
            }
        }
    } else {
        if(wf_alterarEstado($docid, $aedid, $_POST['cmddsc'], array())) {
            exit("
                <script type=\"text/javascript\">
            		alert('Estado alterado com sucesso!');
            		window.opener.location.reload();
                </script>
            ");
        }
    }
    
}

?>
<html>
	<head>
		<title>PAR - Cadastro de subação</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css"/>
		<script type="text/javascript" src="../includes/funcoes.js" ></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
		<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>		
		<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
	</head>
	<body style="font-size:7pt;" leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<form method="post" action="" name="combo_alterar_estado_comentario" enctype="multipart/form-data">
				
			<input type="hidden" name="docid" value="<?php echo $docid ?>"/>
			<input type="hidden" name="esdid" value="<?php echo $esdid ?>"/>
			<input type="hidden" name="aedid" value="<?php echo $aedid ?>"/>
			
			<table width="100%" align="center" border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5; font-size: 8pt;">
				
				<!-- NOME DO DOCUMENTO -->
				<tr>
					<td style="background-color: #d0d0d0; text-align: center;" colspan="2">
						<b><?php echo $documento['docdsc']; ?></b>
					</td>
				</tr>
				
				<!-- ESTADO ATUAL -->
				<tr>
					<td width="20%" align="right" class="subtitulodireita" style="background-color: #dfdfdf;">
						Estado atual
					</td> 
					<td width="80%">
						<?php echo $acao['esddscorigem']; ?>
					</td>
				</tr>
				
				<!-- AÇÂO -->
				<tr>
					<td align="right" class="subtitulodireita" style="background-color: #dfdfdf;">
						Ação
					</td> 
					<td>
						<?php echo $acao['aeddscrealizar']; ?>
					</td>
				</tr>
				
				<!-- COMENTÁRIO -->
				<tr>
					<td align="right" class="subtitulodireita" style="background-color: #dfdfdf;">
						Comentário
					</td> 
					<td>
						<?php
						echo campo_textarea(
							"cmddsc",		// nome do campo
							"S",			// obrigatorio
							"S",			// habilitado
							"Comentário",	// label
							61,				// colunas
							17,				// linhas
							6000			// quantidade maximo de caracteres
						)
						?>
					</td>
				</tr>

				<!-- ANEXO -->
				<tr>
					<td align="right" class="subtitulodireita" style="background-color: #dfdfdf;">
						Anexo
					</td> 
					<td>
						<input type="file" name="arquivo" id="arquivo" />
					</td>
				</tr>
				
				<tr>
					<td colspan="2" style="text-align: center; background-color: #d0d0d0;">
                        <input type="submit" name="alterar_estado" value="Tramitar" onclick="alertaComentario();" style="font-size: 8pt;"/>
					</td>
				</tr>
				
			</table>
		</form>
	</body>
</html>