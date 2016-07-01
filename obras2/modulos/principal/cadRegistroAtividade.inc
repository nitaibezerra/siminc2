<?php
// empreendimento || obra || orgao
verificaSessao( 'obra' );

$regAtividade = new RegistroAtividade( $_REQUEST['rgaid'] );

switch ( $_POST['requisicao'] ){
    case 'salvar':
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";        
        
//        ver($_REQUEST);
//        ver(empty($_REQUEST['rgaid']),d);
//        ver($regAtividade->rgaautomatica);
//        ver(possuiPerfil(PFLCOD_SUPER_USUARIO),d);
        
        if($_REQUEST['rgaid'] && $regAtividade->rgaautomatica === 'f'  && possuiPerfil(PFLCOD_SUPER_USUARIO)){
            $arDado = $_POST;
            $arqid = null;
            $arquivo = $_FILES["arquivo"];
            if ($_FILES["arquivo"] && $arquivo["name"] && $arquivo["type"] && $arquivo["size"]) {
//                include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
                $file = new FilesSimec(null, null, "obras2");
                $file->setPasta('obras2');
                $file->setUpload(null, 'arquivo', false);
                $arqid = $file->getIdArquivo();
            }
            $arDado['arqid'] = ($arqid ? $arqid : $arDado['arqid']);

            if ($_GET['acao'] == 'A') {
                $arDado['empid'] = $_SESSION['obras2']['empid'];
            } else {
                $arDado['obrid'] = $_SESSION['obras2']['obrid'];
            }
            $arDado['usucpf'] = $_SESSION['usucpf'];

            $arCamposNulo = array();
            if (empty($arDado['rgadscsimplificada'])) {
                $arDado['rgadscsimplificada'] = null;
                $arCamposNulo[] = 'rgadscsimplificada';
            }
            if (empty($arDado['rgadsccompleta'])) {
                $arDado['rgadsccompleta'] = null;
                $arCamposNulo[] = 'rgadsccompleta';
            }
            if (empty($arDado['arqid'])) {
                $arDado['arqid'] = null;
                $arCamposNulo[] = 'arqid';
            }

            $regAtividade->popularDadosObjeto($arDado)
                         ->salvar(true, true, $arCamposNulo);
            $db->commit();

            die("<script>
                        alert('Operação realizada com sucesso!'); 
                        window.opener.location.replace( window.opener.location ); 
                        window.close();
                 </script>");
        }elseif(empty($_REQUEST['rgaid'])){
            $arDado = $_POST;
            $arqid = null;
            $arquivo = $_FILES["arquivo"];
            if ($_FILES["arquivo"] && $arquivo["name"] && $arquivo["type"] && $arquivo["size"]) {
//                include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
                $file = new FilesSimec(null, null, "obras2");
                $file->setPasta('obras2');
                $file->setUpload(null, 'arquivo', false);
                $arqid = $file->getIdArquivo();
            }
            $arDado['arqid'] = ($arqid ? $arqid : $arDado['arqid']);

            if ($_GET['acao'] == 'A') {
                $arDado['empid'] = $_SESSION['obras2']['empid'];
            } else {
                $arDado['obrid'] = $_SESSION['obras2']['obrid'];
            }
            $arDado['usucpf'] = $_SESSION['usucpf'];

            $arCamposNulo = array();
            if (empty($arDado['rgadscsimplificada'])) {
                $arDado['rgadscsimplificada'] = null;
                $arCamposNulo[] = 'rgadscsimplificada';
            }
            if (empty($arDado['rgadsccompleta'])) {
                $arDado['rgadsccompleta'] = null;
                $arCamposNulo[] = 'rgadsccompleta';
            }
            if (empty($arDado['arqid'])) {
                $arDado['arqid'] = null;
                $arCamposNulo[] = 'arqid';
            }

            $regAtividade->popularDadosObjeto($arDado)
                         ->salvar(true, true, $arCamposNulo);
            $db->commit();

            die("<script>
                        alert('Operação realizada com sucesso!'); 
                        window.opener.location.replace( window.opener.location ); 
                        window.close();
                 </script>");
        }
        else{
            die("<script>
                    alert('Você não possui permissão para alterar esse Registro!!'); 
                    window.close();
                 </script>");
        }
    case "download":
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$arqid = $_REQUEST['arqid'];
		$file = new FilesSimec();
	    $arquivo = $file->getDownloadArquivo($arqid);
		die();
}

extract( $regAtividade->getDados() );

$arquivo = new Arquivo( $arqid );
?>
<html>
<head>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?php
monta_titulo( 'Registro de Atividade', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
?>
<!--Formulário do Popup-->
<form id="formulario" name="formulario" method="post" action="" enctype="multipart/form-data">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
<?php 
	if ( $rgaautomatica == 't' ):
?>	
		<tr>
			<td class="SubTituloDireita" width="190px"></td>
			<td style="color: red;">
			A restrição foi inserida automáticamente pelo sistema e não será possível sua edição.
			</td>
		</tr>
<?php
	endif;
?>
        <tr>
            <td class="SubTituloDireita" width="190px">Tipo do registro:</td>
            <td>
                <?php
                    $tipos = $db->carregar('SELECT traid as codigo, tranome as descricao FROM obras2.tiporegistroatividade');
                    $db->monta_combo("traid", $tipos, "S", "", "", "", "", "", "N", "traid");
                ?>
            </td>
        </tr>
		<tr>
			<td class="SubTituloDireita" width="190px">Descrição simplificada:</td>
			<td>
			<?php
			$rgadscsimplificada = trim( $rgadscsimplificada ); 
			echo campo_texto( 'rgadscsimplificada', 'N', 'S', '', 47 , 60, '', '');	
			?> 
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
			<td>
				<input type="hidden" name="arqid" id="arqid" value="<?php echo $arquivo->arqid; ?>"/>
				<input type="file" name="arquivo" id="arquivo"/>
				<br>
				<?php
				if ( $arquivo->arqid ){
					echo "<a href='javascript:DownloadArquivo(" . $arquivo->arqid . ");'>(" . $arquivo->arqnome . "." . $arquivo->arqextensao . ")</a>";
				}
				?>
			</td>
		</tr>
		<tr>	
			<td class="SubTituloDireita" width="190px">Descrição detalhada: </td>
			<td>
			<?php
			$rgadsccompleta = trim( $rgadsccompleta );	
			echo campo_textarea( 'rgadsccompleta', 'N', 'S', '', 100, 20, 5000 );  
			?>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td></td>
			<td>
			<div style="float: left;"> 
<?php 
	if ( $rgaautomatica != 't' ):
?>	
				<input type="hidden" name="requisicao" id="requisicao" value="salvar"/>
				<input type="submit" value="Salvar" style="cursor: pointer"/>
<?php
	endif;
?>				
				<input type="button" value="Fechar" style="cursor: pointer" onclick="window.close();">
			</div>
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">
<!--
DownloadArquivo = function(arqid){
	$('#requisicao').val('download');
	$('#arqid').val(arqid);
	$('#formulario').submit();
}
//-->
</script>
</body>
</html>


