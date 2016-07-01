<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if(!$_SESSION['conid']){
	?>
	<script>
		alert("É necessário preencher a tela 'Cadastrar Termo de Compromisso.'\n ou sua sessão expirou. Por favor, entre novamente no PSE!");
		location.href='pse.php?modulo=principal/cadastroTermoCompromisso&acao=A';
	</script>
	<?
	exit;
}


if($_REQUEST['download'] == 'S'){
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
	ob_clean();
    $arquivo = $file->getDownloadArquivo($arqid);
    //echo"<script>window.location.href = 'pse.php?modulo=principal/termoDocumento&acao=A';</script>";
    exit;
} 
if( $_FILES["arquivo"] && !$_POST['arqid']){
	
	$file = new FilesSimec("contratualizacao", $campos ,"pse");
	$arquivoSalvo = $file->setUpload('Termo de Compromisso', '', false);
	
	if($arquivoSalvo){
		$sql = "UPDATE pse.contratualizacao SET arqid = ".$file->getIdArquivo()." WHERE conid = ".$_SESSION['conid'];
		$db->executar( $sql );
		
		//$sql = "DELETE FROM pse.apoiomunicipio WHERE codigoibge = '".$_SESSION['pse']['muncod']."'";
    	//$db->executar($sql);
    
		$db->commit();
		
		echo '<script type="text/javascript"> alert(" Operação realizada com sucesso!");</script>';
		echo"<script>window.location.href = 'pse.php?modulo=principal/termoDocumento&acao=A';</script>";
		die;	
	}
}
if($_GET['arqidDel']){
    $sql = "UPDATE pse.contratualizacao SET arqid = NULL WHERE conid = ".$_SESSION['conid'];
    $db->executar($sql);
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    
	$sql = "select codigoibge from pse.apoiomunicipio where codigoibge = '{$_SESSION['pse']['muncod']}'";
	$codigoibge = $db->pegaUm($sql);
	if(!$codigoibge){    
	    $sql = "INSERT INTO pse.apoiomunicipio(codigoibge) VALUES ('".$_SESSION['pse']['muncod']."')";
	    $db->executar($sql);
	}
	    
    $db->commit();
    
    $file = new FilesSimec();
	$file->excluiArquivoFisico($_GET['arqidDel']);
	
    echo '<script type="text/javascript"> 
    		alert("Operação realizada com sucesso!");
    		window.location.href="pse.php?modulo=principal/termoDocumento&acao=A";
    	  </script>';
    die;
}

$sql = "SELECT mundescricao as municipio, estuf as uf
		FROM territorios.municipio
		WHERE muncod = '".$_SESSION['pse']['muncod']."'";
$mun = $db->pegaLinha($sql);
/*
$sql = "SELECT m.mpaacao06comp1 FROM pse.metapactuada m WHERE m.conid = {$_SESSION['conid']}";
$mpaacao06comp1 = $db->pegaUm($sql);
*/
//pega entid da SECRETARIA MUNICIPAL DE EDUCAÇÃO de um determinado municipio
/*
$sql = "select u.entid, e.entnome 
		from seguranca.usuario u
		left join entidade.entidade e on e.entid = u.entid
		where u.usucpf='".$_SESSION['usucpf']."'";
*/
$sql = "select et.entid, et.entnome from entidade.entidade et
		inner join entidade.endereco ed on ed.entid=et.entid
		inner join entidade.funcaoentidade fe on  fe.entid = et.entid AND fe.funid = 7
		where fe.fuestatus='A' and muncod = '".$_SESSION['pse']['muncod']."'
		order by 1 limit 1";
$secretaria = $db->pegaLinha($sql);

$entid = $secretaria['entid'];
$conano = $_SESSION["exercicio"];



// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';


$titulo = "PSE - Termo de Compromisso";
$dsctitulo = '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.<br>
<table width="100%" align="center"  cellpadding="2" cellspacing="1">
<tr>
	<td width="10%" style="text-align: right;" class="SubTituloDireita">UF:</td>
	<td width="90%" class="SubTituloEsquerda">'.$mun['uf'].'</td>
</tr>
<tr>
	<td style="text-align: right;" class="SubTituloDireita">MUNICÍPIO:</td>
	<td class="SubTituloEsquerda">'.$mun['municipio'].'</td>
</tr>
<tr>
	<td style="text-align: right;" class="SubTituloDireita">SECRETARIA:</td>
	<td class="SubTituloEsquerda">'.$secretaria['entnome'].'</td>
</tr>
</table>';

echo "<br>";

$menu = array(0 => array("id" => 1, "descricao" => "Termo de Compromisso", 			"link" => "/pse/pse.php?modulo=principal/termoDocumento&acao=A"),
					  1 => array("id" => 2, "descricao" => "Semana Saúde na Escola", 	"link" => "/pse/pse.php?modulo=principal/cadastroTermoSemana&acao=A")
				  	  );
		
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

monta_titulo( $titulo, $dsctitulo );
echo "<br>";

$db->cria_aba( $abacod_tela, $url, '');
monta_titulo( 'Anexos de Documento', '' );

$sql = "SELECT arqid FROM pse.contratualizacao WHERE conid = ".$_SESSION['conid'];
$arqid = $db->pegaUm( $sql );


//verifica sessao edição
$dataAtual = date('Y').date('m').date('d');
$sql = "select codigoibge from pse.apoiomunicipio where codigoibge = '{$_SESSION['pse']['muncod']}'";
$codigoibge = $db->pegaUm($sql);
if(($dataAtual >= 20111215 && $dataAtual <= 20120119) && $codigoibge) $_SESSION['permissaoedicao'] = true;


if($arqid) $_SESSION['permissaoedicao'] = false;


//pega array de perfis
$arPerfil = pegaPerfil( $_SESSION['usucpf'] );
?>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript">

	/**
	 * Submete o formulário
	 * @name enviaArquivos
	 * @return void
	 */
	function enviaArquivos( arquivo ){
		var formulario = $('formulario');
	   meuerro = "";
	   if (!arquivo) {
	      //Se não tenho arquivo, é porque não se selecionou um arquivo no formulário.
	        meuerro = "Por favor, selecionar o arquivo que deseja anexar para realizar essa ação.";
	   }else{
	   		if( $('arqid').getValue() != '' ){
	   			meuerro = 'Já foi transferido um TERMO DE COMPROMISSO!';
	   		} else {
				$('requisicao').setValue('salvar');
	         	formulario.submit();
	         	return 1;
         	}
	   }
	   //se estou aqui é porque não se pode submeter
	   alert (meuerro);
	   return 0;
	}
	function excluirAnexo( arqid ){
	 	if ( confirm( 'Deseja excluir o Documento?' ) ) {
	 		location.href= window.location+'&arqidDel='+arqid;
	 	}
	 }
</script>
<form name="formulario" id="formulario" method="post" enctype="multipart/form-data">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="arqid" id="arqid" value="<?=$arqid; ?>">
    	<table class="tabela" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
    		<tr>
    			<td class="SubtituloDireita" style="width: 33%">Termo de Compromisso (anexar arquivo)</td>
    			<td><input type="file" name="arquivo" id="arquivo" />
    				<? if($arPerfil == SUPER_USUARIO || $arPerfil == MEC){?>
    						<input type="button" name="botao" value="Enviar" onclick="enviaArquivos(this.form.arquivo.value);"/>
    				<?}elseif($_SESSION['permissaoedicao'] && !$arqid){
    					
    					
	    				if($_SESSION['conid']){
							$sql = "SELECT  m.mpaid,
											m.mpaacao06comp1,
											m.mpaacao01comp3,
											m.mpaacao02comp3,
											m.mpaacao03comp3,
											m.mpaacao04comp3,
											m.mpaacao05comp3
								     FROM 
								     		pse.metapactuada m
								     WHERE 
								     		m.conid = {$_SESSION['conid']}";
							$dados = $db->carregar($sql);
							if($dados) extract($dados[0]);
							
							$sql = "SELECT sum(pamquanteduseratend) as total FROM pse.planoacaoc1 
									WHERE conid = {$_SESSION['conid']}";
							$concoberturacomp1 = $db->pegaUm($sql);
							
							$sql = "SELECT sum(pacquanteduseratend) as total FROM pse.planoacaoc2 
									WHERE conid = {$_SESSION['conid']}";
							$concoberturacomp2 = $db->pegaUm($sql);
							
						}    					
    				
    					$sql = "SELECT concoberturacomp1 as total1 FROM pse.contratualizacao 
								WHERE conid = {$_SESSION['conid']}";
						$total1 = $db->pegaUm($sql);
						if(!$total1) $total1 = 0;
					
						$sql = "SELECT sum(pamquanteduseratend) as total2 FROM pse.planoacaoc1 
								WHERE conid = {$_SESSION['conid']}";
						$total2 = $db->pegaUm($sql);
						if(!$total2) $total2 = 0;
				     
				     	$sql = "SELECT concoberturacomp2 as total1 FROM pse.contratualizacao 
								WHERE conid = {$_SESSION['conid']}";
						$total3 = $db->pegaUm($sql);
						if(!$total3) $total3 = 0;
						
						$sql = "SELECT sum(pacquanteduseratend) as total2 FROM pse.planoacaoc2 
								WHERE conid = {$_SESSION['conid']}";
						$total4 = $db->pegaUm($sql);
						if(!$total4) $total4 = 0;
						
						if( ($total1 == $total2 || $total1 < $total2) && ( $total3 == $total4 || $total3 < $total4) ){
				     		if($mpaacao06comp1 != '' && $mpaacao04comp3 != '' && $concoberturacomp1 != '' && $concoberturacomp2 != '' && $_SESSION['permissaoedicao']){?>
							     
							     	<input type="button" name="botao" value="Enviar" onclick="enviaArquivos(this.form.arquivo.value);"/>
							<?}
						}    					
    					
    				}
    				?>
    			</td>
    		</tr>
    	</table>
</form>
<?
$sql = "select arq.arqid, arq.arqnome || '.' || arq.arqextensao as arquivo, arq.arqdescricao, 
			to_char(arq.arqdata, 'DD/MM/YYYY') as arqdata, arq.arqhora, usu.usunome
		from pse.contratualizacao con
			inner join public.arquivo arq on con.arqid = arq.arqid
            inner join seguranca.usuario usu on usu.usucpf = arq.usucpf
where con.conid = ".$_SESSION['conid'];

$arDados = $db->carregar($sql);
$arDados = $arDados ? $arDados : array();

if( $arDados ){
?>
<table class="listagem" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
    <tr>
	    <th>Arquivo</th>
	    <th>Descrição</th>
	    <th>Data</th>
	    <th>Hora</th>
	    <th>Usuário</th>
    </tr>
<?
$count = 1;


foreach($arDados as $dados){
	$excluir = '';
	
	if($arPerfil == SUPER_USUARIO) $excluir = '<img src="../imagens/excluir.gif" onClick="excluirAnexo(\''.$dados['arqid'].'\');" style="border:0; cursor:pointer;" title="Excluir Documento Anexo"> &nbsp;';
	if($arPerfil == MEC) $excluir = '<img src="../imagens/excluir.gif" onClick="excluirAnexo(\''.$dados['arqid'].'\');" style="border:0; cursor:pointer;" title="Excluir Documento Anexo"> &nbsp;';
	
	?>
	<tr>		    
        <td align="left">
        	<?php echo $excluir.' '.$count.' - '; ?><a style="cursor: pointer; color: blue;" onclick="window.location='?modulo=principal/termoDocumento&acao=A&download=S&arqid=<?php echo $dados['arqid'];?>'"><?php echo $dados['arquivo'];?></a>
        </td>
        <td align="left">
        	<?php echo $dados['arqdescricao']; ?>
        </td>
        <td align="center">
        	<?php echo $dados['arqdata']; ?>
        </td>
        <td align="center">
        	<?php echo $dados['arqhora']; ?>
        </td>
        <td align="center">
        	<?php echo $dados['usunome']; ?>
        </td>
    	<?php
    		$count++;
}
?>
</table>
<?} ?>