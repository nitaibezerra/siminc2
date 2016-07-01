<? 
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Simec
   Analista: Alexandre Soares Diniz
   Programador: Alexandre Soares Diniz
   Módulo:sistemas.inc
   Finalidade: permitir o cadastro dos sistemas
   */

$_SESSION['sidid'] = "";

########### AÇÕES Cadastro de sistema ###########
######### Funções de UPLOAD #########
function EnviarArquivo($arquivo,$dados=null,$sidid,$tipo){
	global $db;
	
	if (!$arquivo || !$sidid)
		return false;
		
	if (!$arquivo["size"]) {
	 	die("<script>
	 			alert(\"Problemas no envio do arquivo.\");
	 			history.go(-1);
	 		</script>");
	 }			
		
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
				sisid
			)VALUES(
				'".current(explode(".", $arquivo["name"]))."',
				'".end(explode(".", $arquivo["name"]))."',
				'".$dados["arqdescricao"]."',
				'".$arquivo["type"]."',
				'".$arquivo["size"]."',
				'".date('Y/m/d')."',
				'".date('H:i:s')."',
				'".$_SESSION["usucpf"]."',
				". $_SESSION["sisid"] ."
			) RETURNING arqid;";
	$arqid = $db->pegaUm($sql);
	
	//Insere o registro na tabela demandas.imagens
	$sql = "INSERT INTO demandas.imagens 
			(
				sidid,
				arqid,
				imgtipo,
				imgstatus
			)VALUES(
			    ". $sidid .",
				". $arqid .",
				'".$tipo."',
				'A'
			);";
	$db->executar($sql);
	
	if(!is_dir('../../arquivos/demandas/sistemas/'.floor($arqid/1000))) {
		mkdir(APPRAIZ.'/arquivos/demandas/sistemas/'.floor($arqid/1000), 0777);
	}
	$caminho = APPRAIZ . 'arquivos/demandas/sistemas/'. floor($arqid/1000) .'/'. $arqid;
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
	
	
	//$db->commit();
	return true;

}


function inserirSistema()
{
	global $db;
	
    $sql = sprintf("SELECT
					     COUNT(sidid) as total 
					    FROM
					     demandas.sistemadetalhe 
					    WHERE
					     sidstatus = 'A'
					     AND UPPER(sidabrev) = '%s'",
				strtoupper($_REQUEST['abrev']));
	
				
	$total = $db->pegaUm($sql);
				
	if ($total > 0 ){
		die('<script>
	         	alert(\'Já existe a Sigla "'.strtoupper($_REQUEST['abrev']).'" no Banco de Dados.\nOperação não realizada!\');
	         	history.back();
	      	 </script>');
	}

	if(!$_REQUEST['qtdeusuarios']) $_REQUEST['qtdeusuarios'] = "0";

	///////// Cadastro do sistema //////////	   
    $sql = "INSERT INTO demandas.sistemadetalhe (
    			siddescricao,
    			sidabrev,
    			sidfinalidade,
    			sidrelacionado,
    			sidpublico,
    			sidstatus,
    			sidemail,
			    ssiid, 
			    sidmotivo, 
			    sidimplantacao,
			    sidqtdeusuarios, 
			    siddestaquebanner,
			    siddestaqueaba,
			    sipid,
			    sidurl,
			    sitid,
			    sidoutrastecno,
			    sbdid,
			    sidurldoc,
				sidgestor,
				sidpublicar,
			    unaid 			
    			) 
    			VALUES (".
		   "'".$_REQUEST['dsc']."',".
		   "'".strtoupper($_REQUEST['abrev'])."',".   
		   "'".$_REQUEST['finalidade']."',".
		   "'".$_REQUEST['relacionado']."',".
		   "'".$_REQUEST['publico']."',".
		   "'A',".
		   "'".$_REQUEST['email']."',".
    	   "'".$_REQUEST['situacao']."',".
    	   "'".$_REQUEST['motivo']."',".
    	   "'".$_REQUEST['implantacao']."',".
    	   "'".$_REQUEST['qtdeusuarios']."',".
    	   "'".$_REQUEST['dbanner']."',". 
    	   "'".$_REQUEST['daba']."',".     
    	   "'".$_REQUEST['plataforma']."',".
    	   "'".$_REQUEST['sidurl']."',".
			"'".$_REQUEST['tecnologia']."',".
			"'".$_REQUEST['outras']."',".
			"'".$_REQUEST['sbdid']."',".
			"'".$_REQUEST['urldoc']."',".
    		"'".$_REQUEST['sidgestor']."',".
    		"'".$_REQUEST['publicar']."',".
    	   "'".$_REQUEST['orgao']."') RETURNING sidid";
   $sidid = $db->pegaUm($sql);
  
   
   
   	//insere eixos pde
	if ( $_REQUEST['exoid'] && $_REQUEST['exoid'][0] != '' ){

		for($i=0; $i<count($_REQUEST['exoid']); $i++){
			$sql = "INSERT INTO demandas.sistemaeixos (sidid,exoid)
		    		VALUES (".$sidid.",".$_REQUEST['exoid'][$i].")";
		    $db->executar($sql);
		}
		
	}
   
		
   return $sidid;
	
	
}


if ( ($_REQUEST['act'] == 'inserir') && (! is_array($msgerro)) ):

	//insere dados
	$sidid = inserirSistema();
	
	//envia anexo tela inicial
	$anexoTelaInicial = True;
	if($_FILES['anexotelainicial']['size']>0)	$anexoTelaInicial = EnviarArquivo($_FILES['anexotelainicial'], '', $sidid, 'T');

	//envia anexo logo
	$anexoLogo = True;
	if($_FILES['anexologo']['size']>0) $anexoLogo = EnviarArquivo($_FILES['anexologo'], '', $sidid, 'L');
	
	//comita querys no banco
	$db->commit();
	
 	if($sidid && $anexoTelaInicial && $anexoLogo) {
	 	?>
	    	<script>
	            alert('Operação realizada com sucesso!');
			  	location.href='demandas.php?modulo=sistema/apoio/dadosSistemas&acao=A&sidid=<?=$sidid?>';
	        </script>
		<? 
 	}
 	else {
 		die("<script>
 			alert(\"Problemas no envio do arquivo.\");
 			history.go(-1);
 		</script>");  
 	}
 	die;
	
endif;





include APPRAIZ."includes/cabecalho.inc";
?>
<script language="JavaScript" src="../includes/calendario.js"></script>
<br>
<?
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Cadastro de Sistemas';
monta_titulo($titulo_modulo,'<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');
?>
<div align="center">
<center>
<?
//atribui valores as variaveis
$modulo=$_REQUEST['modulo'] ;//



	
?>

<form method="POST"  name="formulario" enctype="multipart/form-data">

	<input type='hidden' name="modulo" value="<?=$modulo?>">
	<input type='hidden' name="act" value=<?=$act?>>

    <center>
    <table border="0"  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td width="30%" align='right' class="SubTituloDireita">Sistema:</td>
	    <td colspan=2><?=campo_texto('dsc','S','S','',50,100,'','');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Sigla:</td>
	    <td>
	    	<?=campo_texto('abrev','S','S','',15,15,'','');?>
	    </td>
	    <td rowspan=3 align=right>
		   	<div style="width:300px;top:250px;">
				<!-- <fieldset style="background-color:#ffffff;"> -->
				<fieldset>
				<legend> <b>Mensagens</a></b></legend>
					<table width="100%" >
						<tr>
							<td valign="bottom" height="50%" style="padding-right:15px;">
								<?php 
								$sql = "select count(sidid) as total from demandas.sistemadetalhe where siddestaquebanner = 'A'";
								$totalBanner = $db->pegaUm($sql);
								echo "TOTAL DESTAQUE BANNER: <b>".$totalBanner."</b> (LIMITE: <B>".DESTAQUE_BANNER_MAX."</B>)";
								?>
							</td>
						</tr>
						<tr>
							<td valign="bottom" height="50%" style="padding-right:15px;">
								<?php 
								$sql = "select count(sidid) as total from demandas.sistemadetalhe where siddestaqueaba = 'A'";
								$totalAba = $db->pegaUm($sql);
								echo "TOTAL DESTAQUE ABA: <b>".$totalAba."</b> (LIMITE: <B>".DESTAQUE_ABA_MAX."</B>)";
								?>
							</td>
						</tr>
					</table>
				</fieldset>	
			</div>	
	    </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Finalidade:</td>
	    <td colspan=2><?=campo_textarea('finalidade','S','S','',75,3,'');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Relacionado:</td>
	    <td colspan=2><?=campo_textarea('relacionado','N','S','',75,3,'');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Público Alvo:</td>
	    <td colspan=2><?=campo_textarea('publico','S','S','',75,3,'');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Plataforma:</td>
	    <td colspan=2><?php
  	
	    	
	    	$sql = "select 	sipid as codigo, 
               				sipdsc as descricao
						from demandas.sistemaplataforma
						order by descricao";	
	    	                             	    
			$db->monta_combo('plataforma', $sql, 'S', "Selecione...", 'filtraPlataforma', '', '', '150', 'S', 'plataforma',false,null,'Plataforma');?></td>
      </tr>
		<?php $mostraUrl='none';
		 if ($plataforma == '1') $mostraUrl='';
		 ?>
      <tr id=tr_url style="display:<?=$mostraUrl?>;">
        <td align='right' class="SubTituloDireita">Url:</td>
	    <td  colspan=2 id=td_url>
	     <?php
	     if (!$sidurl) $sidurl =  '';
	     ?>
	    <?=campo_texto('sidurl','S',$sidid ? 'S' : 'S','',50,100,'','','','','','','',$sidurl);?>
	    </td>
      </tr>

      <tr>
        <td align='right' class="SubTituloDireita">Linguagem:</td>
	    <td  colspan=2><?php

	    	//$sql = "select 	sitid as codigo, sitdsc as descricao from demandas.sistematecnologia where sitid  not in (6) order by 2";	
			//$arraysql = $db->carregar( $sql );	    	                             	    
		
			//$db->monta_combo('tecnologia', $arraysql, 'S', "Selecione...", '', '', '', '100', 'S', 'tecnologia',false,null,'Tecnologia');
			
		?>
		
			<select style="width: 200px;" class="CampoEstilo" name="tecnologia">
			    <option value=''>Selecione...</option>
				<optgroup label="Utilizadas pelo MEC">
					<?php
						$sql = sprintf( "select 	sitid as codigo, sitdsc as descricao from demandas.sistematecnologia where sitid  not in (6) order by 2" );
					?>
					<?php foreach( $db->carregar( $sql ) as $tipo ): ?>
					<option value="<?= $tipo['codigo'] ?>"><?= $tipo['descricao'] ?></option>
					<?php endforeach; ?>
				</optgroup>
				<optgroup label="Outras">
					<?php
						$sql = sprintf( "select 	sitid as codigo, sitdsc as descricao from demandas.sistematecnologia where sitid  in (6) order by 2" );
					?>
					<?php foreach( $db->carregar( $sql ) as $tipo ): ?>
					<option value="<?= $tipo['codigo'] ?>"><?= $tipo['descricao'] ?></option>
					<?php endforeach; ?>
				</optgroup>
			</select>		
			<?php echo obrigatorio();?>
		</td>
			
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Banco de Dados:</td>
	    <td colspan=2><?php

	    	$sql = "select 	sbdid as codigo, 
               				sbddsc as descricao
						from demandas.sistemasgbd
						where sbdstatus = 'A'
						order by 1";	
	    	                             	    
			$db->monta_combo('sbdid', $sql, 'S', "Selecione...", '', '', '', '150', 'S', 'sbdid',false,null,'Banco de Dados');?></td>
      </tr>      
      <tr >
        <td align='right' class="SubTituloDireita">Detalhar Outras Tecnologias:</td>
	    <td colspan=2 >
		    <?=campo_textarea('outras','N','S','',75,3,'');?>
	    </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Url da documentação:</td>
	    <td colspan=2>
	    	<?=campo_texto('urldoc','S','S','',50,120,'','','','','','','',$urldoc);?>
	    </td>
      </tr>      

      <tr>
        <td align='right' class="SubTituloDireita">Situação:</td>
	    <td colspan=2><?php

	    	$sql = "select 	ssiid as codigo, 
               				ssidsc as descricao
						from demandas.sistemasituacao
						order by 1";	
	    	                             	    
			$db->monta_combo('situacao', $sql, 'S', "Selecione...", 'filtraSituacao', '', '', '150', 'S', 'situacao',false,null,'Situacao');?></td>
      </tr>
      	<?php $mostraMotivo='none';
		 if ($situacao == '4') $mostraMotivo='';
		 ?>
      
      <tr id=tr_motivo style="display:<?=$mostraMotivo?>;">
        <td align='right' class="SubTituloDireita">Justificativa Situação:</td>
	    <td colspan=2 id=td_motivo>     
	    <?php 
	    if (!$motivo) $motivo =  '';
	    ?>
			<?=campo_textarea('motivo','S','S','',75,3,'');?>	    
	    </td>
      </tr>
      	<?php $mostraQtde='none';
   	
		 if ($situacao == '2') {
		 	$mostraQtde='';
         		 }
         else  	$qtdeusuarios='0';		 
	 		 
		 ?>

      <tr id=tr_qtde style="display:<?=$mostraQtde?>;">
        <td align='right' class="SubTituloDireita">Quantidade de Usuários:</td>
	    <td colspan=2 id=td_qtde><?=campo_texto('qtdeusuarios','S','S','',15,15,'#########','');?></td>
      </tr>

      <tr>
        <td align='right' class="SubTituloDireita">Data Implantação:</td>
	    <td colspan=2><?=campo_texto('implantacao','S','S','',8,15,'##/####','');?> (mm/aaaa)</td>
      </tr>

      <tr>
        <td align='right' class="SubTituloDireita">Órgão Responsável:</td>
	    <td colspan=2><?php

	    	$sql = "select 	unaid as codigo, 	
                			unasigla || ' - ' || unadescricao as descricao
						from demandas.unidadeatendimento
						order by unasigla";
	    	                             	    
			$db->monta_combo('orgao', $sql, 'S', "Selecione...", '', '', '', '500', 'S', 'orgao',false,null,'orgao');?></td>
      </tr>
      
      <tr>
        <td align='right' class="SubTituloDireita">E-mail:</td>
	     <td colspan=2><?=campo_texto('email', 'N', 'S' , '', 50, 100, '', '', 'left', '', 0, 'id="email"'); ?></td>
      </tr>

      <tr>
        <td align='right' class="SubTituloDireita">Eixo PDE:</td>
	     <td colspan=2>
			<?php

				$stSql = "select 	exoid as codigo, 	
                			exodsc as descricao
						from painel.eixo
						order by 2";
				//mostrarComboPopup( 'Eixo do PDE:', 'exoid',  $stSql, '', 'Selecione o(s) Eixo(s)' );
				//combo_popup( 'unpcod', $sql, 'Selecione a(s) Unidade(s) Parceira(s)', '360x460','','','','S','','',6,400,'pegaNumerosPIs()','pegaNumerosPIs()' );
				//combo_popup( 'exoid', $stSql, 'Selecione o(s) Eixo(s)', '360x460','','','','S','','',6,400,'','' );
				combo_popup( "exoid", $stSql, "Selecione o(s) Eixo(s)", "300x300", 0, array(), "", "S", false, false, 5, 400 );
				
				echo obrigatorio();
	    
			?>
	     
	     </td>
      </tr>
      
      <tr>
        <td align='right' class="SubTituloDireita">Gestor do sistema:</td>
	    <td colspan=2>
	    	<?=campo_texto('sidgestor', 'S', 'S' , '', 50, 100, '', '', 'left', '', 0, 'id="sidgestor"'); ?>
	    </td>
      </tr>
      
      <tr>
        <td align='right' class="SubTituloDireita">Publicar em Relatório:</td>
	    <td colspan=2>
            <input type="radio" name="publicar" value="S" <?=($publicar=='S'?"CHECKED":"")?>> SIM
            <input type="radio" name="publicar" value="N" <?=(($publicar=='N' || $publicar=='')?"CHECKED":"")?>> NÃO
        </td>
      </tr>
      
      <tr>
        <td align='right' class="SubTituloDireita">Destaque Banner:</td>
	    <td colspan=2>
            <input type="radio" name="dbanner" value="A" > SIM
            <input type="radio" name="dbanner" value="I" CHECKED> NÃO
        </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Destaque Aba:</td>
	    <td colspan=2>
            <input type="radio" name="daba" value="A" > SIM
            <input type="radio" name="daba" value="I" CHECKED> NÃO
        </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Imagem Tela Inicial:</td>
	    <td colspan=2>
            <input type="file" name="anexotelainicial" size="50" value="">
        </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Imagem Logomarca:</td>
	    <td colspan=2>
            <input type="file" name="anexologo" size="50" value="">
        </td>
      </tr>
	

	<tr bgcolor="#CCCCCC">
   		<td></td>
   		<td colspan=2>
   			<input type="button" name="btinserir" value="Salvar" onclick="validar_cadastro('I')" class="botao">
   			&nbsp;
   			<input type="button" name="btcancela" value="Limpar" onclick="inicio();" class="botao">
   		</td>
	</tr>

      
    </table>

  
</form>


<script>

var d=document;

function strSpecial(string){
	var search = 'áâàãÁÂÀÃéêèÉÊÈíîìÍÎÌóõòõÓÔÌÕúûùÚÛÙ!@#$%¨&*()-+][}{~^´`?:;,"';

	for (i = 0; i < search.length; i++){
		if ( string.indexOf(search.substr(i,1)) >= 0 ){
			alert('Esse caractere é inválido para o campo: " '+search.substr(i,1)+' "');
			return false;
		}		
	}
	return true;
}



function inicio() {
 	document.formulario.act.value = '';
 	location.href='demandas.php?modulo=sistema/apoio/cadSistemas&acao=A';
}
  
function validar_cadastro() {

	if (trim(document.formulario.dsc.value) == "") 
	{
		alert("Favor informar Nome do Sistema.");
		document.formulario.dsc.focus();
		return;
	}
	
	if (trim(document.formulario.abrev.value) == "") 
	{
		alert("Favor informar Sigla do Sistema.");
		document.formulario.abrev.focus();
		return;
	}
	
	if (trim(document.formulario.finalidade.value) == "") 
	{
		alert("Favor informar Finalidade do Sistema.");
		document.formulario.finalidade.focus();
		return;
	}

/*	
	if (document.formulario.relacionado.value == "") 
	{
		alert("Favor informar relacionamento do Sistema.");
		document.formulario.relacionado.focus();
		return;
	}
*/

	if (trim(document.formulario.publico.value) == "") 
	{
		alert("Favor informar Público Alvo do Sistema.");
		document.formulario.publico.focus();
		return;
	}

	if (trim(document.formulario.plataforma.value) == "")
	{
		alert("Favor informar plataforma do Sistema.");
		document.formulario.plataforma.focus();
		return;		
	}
	
	
	if (document.formulario.plataforma.value == 1){	 
		if (document.formulario.sidurl.value == "http:\\\\" || trim(document.formulario.sidurl.value) == "")  {
			alert("Plataforma WEB é obrigatório informar URL.");
			document.formulario.sidurl.focus();
			return;
		}
	}

	if (trim(document.formulario.tecnologia.value) == "")
	{
		alert("Favor informar Linguagem do Sistema.");
		document.formulario.tecnologia.focus();
		return;		
	}

	if (trim(document.formulario.sbdid.value) == "")
	{
		alert("Favor informar o Banco de Dados do Sistema.");
		document.formulario.sbdid.focus();
		return;		
	}
/*	 
	if (document.formulario.outras.value == "")  {
		alert("Favor informar Detalhar Outras Tecnologias do Sistema.");
		document.formulario.outras.focus();
		return;
	}
*/
	if (document.formulario.urldoc.value == "http:\\\\" || trim(document.formulario.urldoc.value) == "")  {
		alert("Favor informar a URL da documentação do Sistema.");
		document.formulario.urldoc.focus();
		return;
	}	
	
	
	
	if (trim(document.formulario.situacao.value) == "")
	{
		alert("Favor informar situação do Sistema.");
		document.formulario.situacao.focus();
		return;		
	}

	if (document.formulario.situacao.value == 4) {
		if (trim(document.formulario.motivo.value) == "") 
		{
			alert("Situação 'Suspenso' é obrigatório informar Motivo.");
			document.formulario.motivo.focus();
			return;
		}
	}

	if (document.formulario.situacao.value == 2) {
		if (document.formulario.qtdeusuarios.value == 0)
		{
			alert("Situação 'Em Produção' é obrigatório informar a quantidade de usuários.");
			document.formulario.qtdeusuarios.focus();
			return;		
		}
	}
	
	if (trim(document.formulario.implantacao.value) == "")
	{
		alert("Favor informar Data de Implantação do Sistema.");
		document.formulario.implantacao.focus();
		return;		
	}
	var mes = document.formulario.implantacao.value.substring(0,2);
	var ano = document.formulario.implantacao.value.substring(3,7);
	
	if (mes < 1 || mes > 12){
		alert("Data de implantação inválida.");
		document.formulario.implantacao.focus();
		return;		
	}		

	if (ano > 2030 || ano < 1980){
		alert("Data de implantação inválida.");
		document.formulario.implantacao.focus();
		return;		
	}		
	
	if (trim(document.formulario.orgao.value) == "")
	{
		alert("Favor informar Órgão responsável pelo Sistema.");
		document.formulario.orgao.focus();
		return;		
	}


	var exoid = document.getElementById( 'exoid' );
	if ( !exoid[0].value ){
		alert( 'Favor selecionar ao menos um Eixo PDE.' );
		return;
	}

	
	if (trim(document.formulario.sidgestor.value) == "")
	{
		alert("Favor informar o Gestor do Sistema.");
		document.formulario.sidgestor.focus();
		return;
	}
	

	
	if(document.formulario.dbanner[0].checked == true){

		var total = <?=$totalBanner+1?>;

		if( total > <?=DESTAQUE_BANNER_MAX?> ){
			alert("Limite máximo ultrapassado do DESTAQUE BANNER.");
			document.formulario.dbanner[1].checked = true;			
			return;
		}

		if( total < <?=DESTAQUE_BANNER_MIN?> ){
			alert("Quantidade total de DESTAQUE BANNER não pode ser menor que <?=DESTAQUE_BANNER_MIN?>.");
			document.formulario.dbanner[0].checked = true;			
			return;
		}
		
	}
	if(document.formulario.daba[0].checked == true){

		var total = <?=$totalAba+1?>;

		if( total > <?=DESTAQUE_ABA_MAX?> ){
			alert("Limite máximo ultrapassado do DESTAQUE ABA.");
			document.formulario.daba[1].checked = true;			
			return;
		}

		if( total < <?=DESTAQUE_ABA_MIN?> ){
			alert("Quantidade total de DESTAQUE ABA não pode ser menor que <?=DESTAQUE_ABA_MIN?>.");
			document.formulario.daba[0].checked = true;			
			return;
		}
		
	}
	
		
	selectAllOptions( exoid );

	document.formulario.act.value = 'inserir';
    document.formulario.submit();

}



function filtraPlataforma(sipid) {
	
	trUrl  = d.getElementById('tr_url');
	tdUrl  = d.getElementById('td_url');

	trUrl.style.display = 'none';
	tdUrl.style.display = 'none';

	if(sipid == 1){ //Web
		trUrl.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		tdUrl.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
	}
}


function filtraSituacao(ssiid) {
	
	trMotivo  = d.getElementById('tr_motivo');
	tdMotivo  = d.getElementById('td_motivo');
	trQtde	  = d.getElementById('tr_qtde');
	tdQtde 	  = d.getElementById('td_qtde');
	

	trMotivo.style.display = 'none';
	tdMotivo.style.display = 'none';
	trQtde.style.display = 'none';
	tdQtde.style.display = 'none';
	

	if(ssiid == 2){ //Em Produção
		trQtde.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		tdQtde.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
	}

	if(ssiid == 4){ //Suspenso
		trMotivo.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		tdMotivo.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
	}
	
}

</script>
