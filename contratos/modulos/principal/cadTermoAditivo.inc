<?php
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

include_once APPRAIZ . "includes/classes/dateTime.inc";
 
//verifica sessão da pagina $_SESSION['ctrid']
verificaSessaoPagina();



if($_REQUEST['tdaid'] && $_REQUEST['op'] == 'Excluir'){
	$sql = "UPDATE contratos.cttermoaditivo SET tdastatus = 'I' WHERE tdaid = ".$_REQUEST['tdaid'];
	$db->executar($sql);
	$db->commit();
	$_REQUEST['acao'] = "A";
	$db->sucesso('principal/cadTermoAditivo', "");
	die;
}

if($_POST['action']){
	salvar();
}

function salvar(){
	global $db; 
 	 
	$formataDatamA = formata_data_sql( $_REQUEST['tdadata'] );
	$formataDatamI = formata_data_sql( $_REQUEST['tdainiciovig'] );
	$formataDatamF = formata_data_sql( $_REQUEST['tdafimvig'] );
	
	$vlr = str_replace(',','.',str_replace('.','',$_REQUEST['tdavlr']));
	if(!$vlr) $vlr = 'NULL';
	else $vlr = "'".$vlr."'";

	
	//verifica se dt fim contrato é maior que data inicio aditivo
	$sql = "SELECT to_char(ctrdtfimvig,'YYYYMMdd') FROM contratos.ctcontrato WHERE ctrid = ".$_SESSION['ctrid'];
	$ctrdtfimvig = $db->pegaUm( $sql );
	$dtinivig = str_replace("-", "", $formataDatamI);
	
	if($ctrdtfimvig > $dtinivig && $_POST['tiaid'] != 2 && $_POST['tiaid'] != 4){
		echo '<script>
					alert("Data Início Vigência deve ser maior que a Data Término do contrato!");
					history.back();
				  </script>';
		die();
	}
	
	
	//verifica se a ultima data fim aditivo é maior que a nova data inicio aditivo
	$sql = "SELECT to_char(tdafimvig,'YYYYMMdd') as dtultima FROM contratos.cttermoaditivo 
			WHERE ctrid = ".$_SESSION['ctrid']."
			and tdastatus = 'A'
			order by tdafimvig desc";
	
	$dtdados = $db->carregar( $sql );
	if($dtdados){

		if(!$_REQUEST['tdaid']){
			$tdafimvig = $dtdados[0]['dtultima'];
		}else{
			$tdafimvig = $dtdados[1]['dtultima'];
		}
		
		$dtinivig = str_replace("-", "", $formataDatamI);
		
		if($tdafimvig > $dtinivig && $_POST['tiaid'] != 2  && $_POST['tiaid'] != 4){
			echo '<script>
						alert("Data Início Vigência deve ser maior que a última Data Fim Vigência!");
						history.back();
					  </script>';
			die();
		}
		
	}
		 
 	if(!$_REQUEST['tdaid']){
        if ($_POST['tiaid'] == 2) {
            $sql = "INSERT INTO
	    		contratos.cttermoaditivo
	    		(
	    		 ctrid,
	    		 tiaid,
				 tdanum,
				 tdadata,
				 tdaobj,
				 tdavlr,
            	 tdastatus,
            	 tdaobs
				)
				VALUES
				(
					'".$_SESSION['ctrid']."',
					".$_POST['tiaid'].",
					'".$_POST['tdanum']."',
					".($formataDatamA ? "'" . $formataDatamA . "'" : "NULL").",
					'".$_POST['tdaobj']."',
					".trim(str_replace('R$','',$vlr)).",
					'A',
					'".$_POST['tdaobs']."'
				)
	    		RETURNING tdaid";
        } else {
            $sql = "INSERT INTO
	    		contratos.cttermoaditivo
	    		(
	    		 ctrid,
	    		 tiaid,
				 tdanum,
				 tdadata,
				 tdaobj,
				 tdavlr,
				 tdainiciovig,
            	 tdafimvig,
            	 tdastatus,
            	 tdaobs
				)
				VALUES
				(
					'".$_SESSION['ctrid']."',
					".$_POST['tiaid'].",
					'".$_POST['tdanum']."',
					".($formataDatamA ? "'" . $formataDatamA . "'" : "NULL").",
					'".$_POST['tdaobj']."',
					".trim(str_replace('R$','',$vlr)).",
					" . ($formataDatamI ? "'" . $formataDatamI . "'" : "NULL") .",
					" . ($formataDatamF ? "'" . $formataDatamF . "'" : "NULL") .",
					'A',
					'".$_POST['tdaobs']."'

				)
	    		RETURNING tdaid";
        }

				 
 	}elseif($_REQUEST['tdaid']){
        if ($_POST['tiaid'] == 2) {
            $sql = "UPDATE contratos.cttermoaditivo
				SET
						 tiaid 			= ".$_POST['tiaid'].",
						 tdanum 		= '".$_POST['tdanum']."',
						 tdadata 		= ".($formataDatamA ? "'" . $formataDatamA . "'" : "NULL").",
						 tdaobj 		= '".$_POST['tdaobj']."',
						 tdaobs 		= '".$_POST['tdaobs']."',
						 tdavlr 		= ".trim(str_replace('R$','',$vlr))."
								WHERE
					tdaid = ".$_REQUEST['tdaid']."
					RETURNING tdaid
		";
        } else {
            $sql = "UPDATE contratos.cttermoaditivo
				SET
						 tiaid 			= ".$_POST['tiaid'].",
						 tdanum 		= '".$_POST['tdanum']."',
						 tdadata 		= ".($formataDatamA ? "'" . $formataDatamA . "'" : "NULL").",
						 tdaobj 		= '".$_POST['tdaobj']."',
						 tdaobs 		= '".$_POST['tdaobs']."',
						 tdavlr 		= ".trim(str_replace('R$','',$vlr)).",
						 tdainiciovig 	= '".$formataDatamI."',
		            	 tdafimvig		= " . ($formataDatamF ? "'" . $formataDatamF . "'" : "NULL") ."
								WHERE
					tdaid = ".$_REQUEST['tdaid']."
					RETURNING tdaid
		";
        }

		
 	}

 	//dbg($sql,1);
	$tdaid = $db->pegaUm($sql);

	$db->commit();

	if (!$_FILES['anexo']['size'] || EnviarArquivo($_FILES['anexo'], '', $tdaid)){ 	
	}
		
			
	$_REQUEST['acao'] = "A";
	$db->sucesso('principal/cadTermoAditivo', "");
	die;
}

######### Funções de UPLOAD #########
function EnviarArquivo($arquivo,$dados=null,$tdaid){
	global $db;

	if (!$arquivo || !$tdaid)
		return false;
		
	// obtém o arquivo
	#$arquivo = $_FILES['arquivo'];
	if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
	}
	// BUG DO IE
	// O type do arquivo vem como image/pjpeg
	if($arquivo["type"] == 'image/pjpeg') {
		$arquivo["type"] = 'image/jpeg';
	}
	//Insere o registro do arquivo na tabela public.arquivo
	$sql = "INSERT INTO public.arquivo 	
			(
				arqnome,
				arqextensao,
				arqdescricao,
				arqtipo,
				arqtamanho,
				arqdata,
				arqhora,
				usucpf,
				sisid,
				arqstatus
			)VALUES(
				'".current(explode(".", $arquivo["name"]))."',
				'".end(explode(".", $arquivo["name"]))."',
				'Termo aditivo de contrato',
				'".$arquivo["type"]."',
				'".$arquivo["size"]."',
				'".date('Y/m/d')."',
				'".date('H:i:s')."',
				'".$_SESSION["usucpf"]."',
				". $_SESSION["sisid"] .",
				'A'
			) RETURNING arqid;";
	$arqid = $db->pegaUm($sql);
	
	//Insere o registro na tabela eveno.ctanexo
	$sql = "INSERT INTO contratos.ctanexo 
			(
	    	ctrid, 
	    	arqid, 
	    	tpaid, 
            ancdatainclusao, 
	    	usucpf, 
	    	ancstatus, 
            tdaid 
			)VALUES(
			    ".$_SESSION['ctrid'].",
				". $arqid .",
				".ID_TERMO_ADITIVO.",
				now(),
				'".$_SESSION["usucpf"]."',
				'A',
				". $tdaid ."
			);";
	$db->executar($sql);
	
	if(!is_dir('../../arquivos/'. $_SESSION['sisdiretorio'] .'/'.floor($arqid/1000))) {
		mkdir(APPRAIZ.'/arquivos/'. $_SESSION['sisdiretorio'] .'/'.floor($arqid/1000), 0777);
	}
	$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
	switch($arquivo["type"]) {
		case 'image/jpeg':
			ini_set("memory_limit", "128M");
			list($width, $height) = getimagesize($arquivo['tmp_name']);
			$original_x = $width;
			$original_y = $height;
			// se a largura for maior que altura
			if($original_x > $original_y) {
  	 			$porcentagem = (100 * 640) / $original_x;      
			}else {
   				$porcentagem = (100 * 480) / $original_y;  
			}
			$tamanho_x = $original_x * ($porcentagem / 100);
			$tamanho_y = $original_y * ($porcentagem / 100);
			$image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
			$image   = imagecreatefromjpeg($arquivo['tmp_name']);
			imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
			imagejpeg($image_p, $caminho, 100);
			//Clean-up memory
			ImageDestroy($image_p);
			//Clean-up memory
			ImageDestroy($image);
			break;
		default:
			if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
				$db->rollback();
				return false;
			}
	}
	
	
	$db->commit();
	return true;

}

######## Fim funções de UPLOAD ########


if( $_REQUEST['tdaid'] ){
		
	$sql = "SELECT 				 
					tda.tdanum,
					to_char(tda.tdadata::date,'YYYY-MM-dd') AS tdadata, 
					tda.tdaobj,
					tda.tdaobs,
					tda.tdavlr  as tdavlr,	
					to_char(tda.tdainiciovig::date,'YYYY-MM-dd') AS tdainiciovig,
					to_char(tda.tdafimvig::date,'YYYY-MM-dd') AS tdafimvig,
					tda.tiaid
				FROM 
					contratos.cttermoaditivo AS tda 
			WHERE
				tda.ctrid = ".$_SESSION['ctrid']."
				AND tda.tdaid = '".$_REQUEST['tdaid']."'
				";
	$rsDadosProcesso = $db->carregar( $sql );

}
// Por padrão os campos vem habilitados
// $desabilitado   = false;
// $somenteLeitura = 'S';
// $verifica = true;
$acesso 		= verificaPermissaoTelaUsuario();
$desabilitado   = $acesso['desabilitado'];
$somenteLeitura = $acesso['leitura'];

$perfis 			= arrayPerfil();
// $arPerfisBloqEdicao = array(PERFIL_CONSULTA_UNIDADE, PERFIL_CONSULTA_GERAL);

// $arInt = array_intersect($perfis, $arPerfisBloqEdicao);

// //MÉTODO PARA A VERIFICAR SE O CONTRATO PERTENCE À UNIDADE DO USUARIO E SE O USUARIO PODERÁ EDITAR NESTA TELA
// if($_SESSION['ctrid'])
// 	$verifica = verificaResponsabilidade($_SESSION['ctrid'],$perfis,array(PERFIL_GESTOR_FINANCEIRO_UNIDADE,PERFIL_EQUIPE_TECNICA_UNIDADE));
// //dbg($verifica,d);
// //verifica se nao esta no bloqueado e se caso nao esteja bloq ele tem direito a editar
// if( count($arInt) != 0 || (count($arInt) == 0 && $verifica==false)) {
// 	$desabilitado 	= true;
// 	$somenteLeitura = 'N';
// }


//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, obrigatorio() . ' Indica campo obrigatório' );
montaCabecalhoContrato($_SESSION['ctrid']);

?> 
<script src="../includes/prototype.js"></script>
<script src="../includes/calendario.js"></script> 

<? if(!$desabilitado) : ?>
    <form name="formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario" enctype="multipart/form-data">
    <?php 
    //Busca a data fim do contrato
    $sql = "SELECT 
				TO_CHAR(ctrdtfimvig,'DD/MM/YYYY') AS dtfimcontrato 
			FROM 
				contratos.ctcontrato 
			WHERE 
				ctrid = ".$_SESSION['ctrid'];
    $dtfimcontrato = $db->pegaUm($sql);
    ?>
    <input type="hidden" name="dtfimcontrato" id="dtfimcontrato" value="<?php echo $dtfimcontrato; ?>">
    
    <?php 
    //Busca a última data fim do aditivo
    $sql = "SELECT 
				TO_CHAR(tdafimvig,'DD/MM/YYYY') AS dtfimaditivo 
			FROM 
				contratos.cttermoaditivo
			WHERE 
				ctrid = ".$_SESSION['ctrid']."
				" . ($_GET['tdaid'] ? "AND tdaid != " . $_GET['tdaid'] : "") . "
				AND tdastatus = 'A'
				AND tdafimvig IS NOT NULL
			ORDER BY 
				tdafimvig DESC
			LIMIT 1";
    $dtfimaditivo = $db->pegaUm($sql);
    ?>
    <input type="hidden" name="dtfimaditivo" id="dtfimaditivo" value="<?php echo $dtfimaditivo; ?>">
    
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        	<tr>	
                <td class ="SubTituloDireita" align="right">Tipo: </td> 
                <td> 
                 <?
				 	$tiaid = $rsDadosProcesso[0]['tiaid'];
                 ?>
            	 <?
	            	 $sqlc = "select tiaid as codigo, tiadescricao as descricao from contratos.tipoaditivo where tiastatus = 'A'";
	            	 
	            	 $db->monta_combo( 'tiaid', $sqlc, $somenteLeitura, '--Selecione o Tipo--','','','','','S','tiaid' );
            	 ?>               
                </td>
            </tr>
        	<tr>	
                <td class ="SubTituloDireita" align="right">Nº Termo: </td> 
                <td> 
                 <?
				 $tdanum = $rsDadosProcesso[0]['tdanum'];
                 ?>
            	 <?= campo_texto('tdanum', 'S', $somenteLeitura, 'Nº Termo', 40, 30, '', '', 'left', '',  0, 'id="tdanum" onblur="MouseBlur(this);"' ); ?>               
                </td>
            </tr>

            <tr id="campoTdaObs" style="display:none">
                <td class="subtitulodireita" valign="top">
                    Observações:
                </td>
                <td align="left">
                    <div style="float:left">
                        <?
                            $tdaobs = $rsDadosProcesso[0]["tdaobs"];
                        ?>
                        <textarea name="tdaobs" title="Observações" rows="15" cols="80"><?= $tdaobs ?></textarea>
                    </div>
                    <div style="float:left">
                        &nbsp;<?php echo obrigatorio();?>
                    </div>
                </td>
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right">Data: </td>
                <td> 
                <div style="float:left"> 
                <?php 
                $tdadata = $rsDadosProcesso[0]["tdadata"];   
                ?>
                <?= campo_data2( 'tdadata','', 'S', 'Data', 'S' ); ?> 
                </div>
                <div id="dataobrigatoria" style="<?php if($tiaid==2) echo'display:none;'?> float:left,">
		      	 	&nbsp;<?php echo obrigatorio();?>
		      	</div>
                </td> 
            </tr>
      	         	 
            
            <tr>
	      		<td class="subtitulodireita" valign="top"> 	
	      	 		Descrição do Objeto:
	      	 	</td>
	      	 	<td align="left">
	      	 	<?php
				$tdaobj = $rsDadosProcesso[0]["tdaobj"];
	      	 	?>
	      	 		<!-- 
		      	 	<textarea name="tdaobj" title="Descrição do Objeto" rows="15" cols="80" class="text_editor_simple"><?//= $tdaobj ?></textarea>
		      	 	-->
					<div style="float:left">
		      	 	<textarea name="tdaobj" id="tdaobj" title="Descrição do Objeto" rows="15" cols="80"><?= $tdaobj ?></textarea>
		      	 	</div>
		      	 	<div style="float:left">
		      	 	&nbsp;<?php echo obrigatorio();?>
		      	 	</div>		      	 	
		      	 </td>
	        </tr>
        	<tr>
                <td class ="SubTituloDireita" align="right">Valor: </td> 
                <td> 
                <div style="float:left">
                 <?
				 $tdavlr = $rsDadosProcesso[0]['tdavlr'];
				 if($tdavlr) $tdavlr = number_format($tdavlr,2,",",".");   
                 ?>
            	 <?= campo_texto( 'tdavlr', 'N', 'S', 'Valor', 17, 15, '###.###.###,##', '', 'right', '', 0, 'id="tdavlr" onblur="MouseBlur(this);"'); ?>
            	 </div>
            	 <div id="valorobrigatorio" style="<?php if($tiaid==1) echo'display:none;'?> float:left" >
		      	 	&nbsp;<?php echo obrigatorio();?>
		      	</div>               
                </td>
            </tr>
            <tr id="dtInicio">
                <td class ="SubTituloDireita" align="right">Data Início Vigência: </td>
                <td>  
                <?php 
                $tdainiciovig = $rsDadosProcesso[0]["tdainiciovig"];   
                ?>
                <?= campo_data2( 'tdainiciovig','S', 'S', 'Data início da vigência', 'S' ); ?> 
                </td> 
            </tr>

            <tr id="dtFim">
                <td class ="SubTituloDireita" align="right">Data Fim Vigência: </td>
                <td>  
                <?php 
                $tdafimvig = $rsDadosProcesso[0]["tdafimvig"];   
                ?>
                <?= campo_data2( 'tdafimvig','S', 'S', 'Data fim da vigência', 'S' ); ?> 
                </td> 
            </tr>

		    <tr>
		        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
		        <td>     	
		 			<input name="anexo" type="file" style="text-align: left; width: 83ex;" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);" onmouseover="MouseOver(this);" class="normal" size="81">	            
		        </td>       
		    </tr>
            
            <tr>
            	<td  class ="SubTituloDireita" align="right"></td>
            	 <td>
            	   <input type="hidden" name="action" value="1" id="action">
            	   <input type="hidden" name="tdaid" value="<?=$_REQUEST['tdaid']; ?>" id="tdaid">
            	   <?if(!$_REQUEST['tdaid']){?>
            	   		<input type="button" name="btnGravar" value="Gravar" id="btnGravar" onclick="enviaForm();">
            	   <?}else{?>
            	   		<input type="button" name="btnGravar" value="Alterar" id="btnGravar" onclick="enviaForm();">
            	   		&nbsp;&nbsp;
            	   		<input type="button" name="btnGravar" value="Novo" id="btnNovo" onclick="window.location.href = 'contratos.php?modulo=principal/cadTermoAditivo&acao=A';">
            	   <?}?>
            	  </td>
            </tr>
      </table>
      </form> 
<? endif; ?>

<?php
/*
$sql = "SELECT 
			DISTINCT 
			('<center>
			".(($desabilitado)?"":"<img align=\"absmiddle\" src=\"/imagens/alterar.gif\"  style=\"cursor: pointer\" onclick=\"javascript: selecionarTermo('|| tda.tdaid ||' );\" title=\"Selecionar Termo Aditivo\"> <img align=\"absmiddle\" src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"removeTermo('''||tda.tdaid||''');\" border=0 alt=\"Ir\" title=\"Excluir\">")."</center>') as acao,
			CASE
			  		WHEN anc.tdaid IS NULL THEN ''
			  		WHEN anc.tdaid IS NOT NULL THEN '<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'?modulo=principal/cadContratoAnexo&acao=A&download=S&arqid=' || arq.arqid || '\';\" /><img src=\"/imagens/anexo.gif\" style=\"cursor: pointer\"> </a>' 
	 		END as doc,
			tda.tdanum,
			to_char(tda.tdadata::date,'dd/MM/YYYY') AS tdadata, 
			tda.tdaobj, 
			coalesce( tda.tdavlr, 0 ) / 1 as tdavlr,
			to_char(tda.tdainiciovig::date,'dd/MM/YYYY') AS tdainiciovig,
			to_char(tda.tdafimvig::date,'dd/MM/YYYY') AS tdafimvig
		    FROM contratos.cttermoaditivo tda
       		LEFT JOIN contratos.ctanexo anc on anc.tdaid = tda.tdaid
       		LEFT JOIN public.arquivo arq on arq.arqid = anc.arqid      			    
			WHERE tda.tdastatus = 'A' and tda.ctrid = {$_SESSION['ctrid']}
			order by 2"; 
			
$cabecalho = array( "&nbsp;&nbsp;&nbsp;&nbsp;Ação"," ", "TA nº", "Data", "Objeto", "Valor", "Vigência Início", "Vigência Término" ); 
*/
// $db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '');


$sql = "SELECT
			DISTINCT
			tda.tdaid,
			CASE
			WHEN anc.tdaid IS NULL THEN ''
			WHEN anc.tdaid IS NOT NULL THEN '<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'?modulo=principal/cadContratoAnexo&acao=A&download=S&arqid=' || arq.arqid || '\';\" /><img src=\"/imagens/anexo.gif\" style=\"cursor: pointer\"> </a>'
			END as doc,
			tda.tdanum,
			to_char(tda.tdadata::date,'dd/MM/YYYY') AS tdadata,
			tda.tdaobj,
			coalesce( tda.tdavlr, 0 ) as tdavlr,
			to_char(tda.tdainiciovig::date,'dd/MM/YYYY') AS tdainiciovig,
			to_char(tda.tdafimvig::date,'dd/MM/YYYY') AS tdafimvig
			FROM contratos.cttermoaditivo tda
			LEFT JOIN contratos.ctanexo anc on anc.tdaid = tda.tdaid
			LEFT JOIN public.arquivo arq on arq.arqid = anc.arqid
			WHERE tda.tdastatus = 'A' and tda.ctrid = {$_SESSION['ctrid']}
			order by 2";

// dbg($sql, d);

$dados = $db->carregar($sql);
?>
<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
	<thead>
	<tr>
		<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" title="&nbsp;&nbsp;&nbsp;&nbsp;Ação" bgcolor="">
			<strong>&nbsp;&nbsp;&nbsp;&nbsp;Ação</strong>
		</td>
		<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" title="">
			<strong>Anexo</strong>
		</td>
		<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" title="TA nº">
			<strong>TA nº</strong>
		</td>
		<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" title="Data" bgcolor="">
			<strong>Data</strong>
		</td>
		<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" title="Objeto" bgcolor="">
			<strong>Objeto</strong>
		</td>
		<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" title="Valor">
			<strong>Valor</strong>
		</td>
		<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" title="Vigência Início">
			<strong>Vigência Início</strong>
		</td>
		<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" title="Vigência Término">
			<strong>Vigência Término</strong>
		</td>
	</tr>
	</thead>
	<tbody>
		<?
		$i = 1;
		if($dados){
			foreach ($dados as $d){
				if ($cor == '#F7F7F7'){
					$cor = '#FFFFFF';
				}else{
					$cor = '#F7F7F7';
				}
				 
			?>
				<tr bgcolor="<?=$cor?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$cor?>';">
					<td align="left">
						<center>
							<?if(count($dados) == $i){?>
								<?php 
								if ($desabilitado != true){
								?>
								<img align="absmiddle" src="/imagens/alterar.gif"  style="cursor: pointer" onclick="javascript: selecionarTermo('<?=$d['tdaid']?>');" title="Editar Termo Aditivo">
								<img align="absmiddle" src="/imagens/excluir.gif " style="cursor: pointer" onclick="removeTermo('<?=$d['tdaid']?>');" border=0 alt="Ir" title="Excluir">
								<?php 
								}
								?>
							<?}?>
						</center>
					</td>
					<td align="left">
						<center><?=$d['doc']?></center>
					</td>
					<td align="left">
						<?=$d['tdanum']?>
					</td>
					<td align="left">
						<center><?=$d['tdadata']?></center>
					</td>
					<td align="left">
						<?=$d['tdaobj']?>
					</td>
					<td align="right">
						<?=($d['tdavlr'] ? number_format($d['tdavlr'],2,",",".") : '0,00')?>
					</td>
					<td align="left">
						<center><?=$d['tdainiciovig']?></center>
					</td>
					<td align="left">
						<center><?=$d['tdafimvig']?></center>
					</td>
				</tr>
			<?
			$i++;
			}
		}else{?>
			<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
					<td align="left" colspan="8">
						<center>
							Não existem registros.
						</center>
					</td>
			</tr>
		<?}?>
		
	</tbody>
</table>      
	
<script	type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript">
jQuery.noConflict();

d = document;
jQuery(d).ready(function(){
    jQuery('#tiaid').change(function(){

			if (jQuery(this).val()=='2' || jQuery(this).val()=='4' || jQuery(this).val()=='5') {
                jQuery('#dtFim').hide();
                jQuery('#dtInicio').hide();
			} else {
                jQuery('#dtFim').show();
                jQuery('#dtInicio').show();
			}

            if (jQuery(this).val() == '4') {
                jQuery('#campoTdaObs').show();
            } else {
                jQuery('#campoTdaObs').hide();
            }

            if (jQuery(this).val() == '5') {
                jQuery('#valorobrigatorio').hide();
            } else {
                jQuery('#valorobrigatorio').show();
            }

	   });	
	});
function selecionarTermo( tdaid )
{   
	window.location.href = 'contratos.php?modulo=principal/cadTermoAditivo&acao=A&tdaid='+tdaid; 
}

function enviaForm(){
	/*
 	var nomeform 		= 'formulario';
	var submeterForm 	= true;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();

	campos[0] 			= "tdanum";
	campos[1] 			= "tdadata";
	campos[2]			= "tdaobj"; 
	campos[3]			= "tdavalor";
	campos[4]			= "tdainiciovig";
	campos[5]			= "tdafimvig";

	tiposDeCampos[0] 	= "texto";
	tiposDeCampos[1] 	= "data"; 
	tiposDeCampos[2] 	= "textarea";
	tiposDeCampos[3] 	= "texto"; 
	tiposDeCampos[4] 	= "data"; 
	tiposDeCampos[5] 	= "data"; 
	

	validaForm(nomeform, campos, tiposDeCampos, submeterForm );
	*/
	var msg = new Array();
	
	if (d.formulario.tiaid.value == ''){
		msg.push("O campo Tipo é obrigatório.");
//		alert("O campo Tipo é obrigatório.");
//		d.formulario.tiaid.focus();
//		return false;
	}

	if (d.formulario.tdanum.value == ''){
		msg.push("O campo Nº Termo é obrigatório.");
//		alert("O campo Nº Termo é obrigatório.");
//		d.formulario.tdanum.focus();
//		return false;
	}
	
	if (d.formulario.tdadata.value == '' && d.formulario.tiaid.value!='2'){
		msg.push("O campo Data é obrigatório.");
//		alert("O campo Data é obrigatório.");
//		d.formulario.tdadata.focus();
//		return false;
	}
	
	if (d.formulario.tdaobj.value == ''){
		msg.push("O campo Descrição do Objeto é obrigatório.");
//		alert("O campo Descrição do Objeto é obrigatório.");
//		d.formulario.tdaobj.focus();
//		return false;
	}
	if (d.formulario.tiaid.value == '2' || d.formulario.tiaid.value == '3' ){
		var valor = new String( d.formulario.tdavlr.value );
		valor 	  = valor.replace(".", "");
		valor 	  = valor.replace(",", "");
		
		if (d.formulario.tdavlr.value == ''&& d.formulario.tiaid.value!='1'){
			msg.push("O campo Valor é obrigatório.");
//			alert("O campo Valor é obrigatório.");
//			d.formulario.tdavlr.focus();
//			return false;
		}else if ( isNaN(valor) ){
			msg.push("O campo Valor só aceita valores numéricos");
		}
	}

    if (d.formulario.tiaid.value == '1' || d.formulario.tiaid.value == '3'){
        if (d.formulario.tdainiciovig.value == ''){
            msg.push("O campo Data Início Vigência é obrigatório.");
    //		alert("O campo Data Início Vigência é obrigatório.");
    //		d.formulario.tdainiciovig.focus();
    //		return false;
        }
    }
	
	if (d.formulario.tiaid.value == '1' || d.formulario.tiaid.value == '3'){
		if (d.formulario.tdafimvig.value == ''){
			msg.push("O campo Data Fim Vigência é obrigatório.");			
//			alert("O campo Data Fim Vigência é obrigatório.");
//			d.formulario.tdafimvig.focus();
//			return false;
		}
	}

	var dataIniVigencia = jQuery('#tdainiciovig').val();
	if ( dataIniVigencia != '' && msg.size() == 0 ){
		var dataFimContrato = jQuery('#dtfimcontrato').val();
        var tipo = jQuery("#tiaid").val();
		var dataFimAditivo  = jQuery('#dtfimaditivo').val();
		
		var ObData = new Data();

		if( ObData.comparaData(dataFimContrato, dataIniVigencia, '>') && tipo != 2 && tipo != 4  && tipo != 5){
			alert("A data de início da vigência do aditivo (" + dataIniVigencia + ") deve ser maior que o término da vigência do contrato (" + dataFimContrato + ")!");
			return false;
		}
		
		if( dataFimAditivo != '' && ObData.comparaData(dataFimAditivo, dataIniVigencia, '>') && tipo != 2 && tipo != 4  && tipo != 5){
			alert("A data de início da vigência do novo aditivo (" + dataIniVigencia + ") deve ser maior que o término da vigência do último aditivo (" + dataFimAditivo + ")!");
			return false;
		}
	}	

	if (msg.size() > 0){
		alert( msg.join("\n") );
	}else{
		d.formulario.submit();
	}	
}

function selecionaArquivo( id ){
	var req = new Ajax.Request('contratos.php?modulo=principal/cadTermoAditivo&acao=A', {
					        method:     'post',
					        parameters: '&ajaxsession=' + id,							         
					        onComplete: function (res) {	
								window.location.href = '?modulo=principal/cadContratoAnexo&acao=A&download=S&arqid='+id;
							}
		});
}


function removeTermo(tdaid){
	if(confirm('Deseja excluir este item?')){
		window.location.href = 'contratos.php?modulo=principal/cadTermoAditivo&acao=A&op=Excluir&tdaid='+tdaid; 
		return true;
	} else {
		return false;
	}
}


</script>