<?php
$_SESSION['academico']['entid'] = $_REQUEST['entidunidade'] ? $_REQUEST['entidunidade'] : $_SESSION['academico']['entid'];
if ( $_REQUEST["carga"] ){
	header('Content-Type: text/html; charset=ISO-8859-1');
	academico_busca_obras( $_REQUEST["carga"], '100%', 'inauguradas' );
	die;
}

if($_REQUEST['exec_function']) {
	$_REQUEST['exec_function']($_REQUEST);
}elseif($_REQUEST['opt']=="buscarCnpj"){
	buscarCnpj($_REQUEST);
}

// Instancia o objeto
$autoriazacaoconcursos = new autoriazacaoconcursos();
verificaExistenciaEntidades();
if ( $_REQUEST['acao'] == 'A' ){
	
	$entid  						 = academico_existeentidade( $_SESSION["academico"]["entid"] );
	$_SESSION['sig_var']['iscampus'] = 'nao';
	$_SESSION["academico"]["entid"]  = $entid;	
	$param	 						 = "&entidunidade=" . $entid;
	$unidade 						 = $entid;
	
}else if ( $_REQUEST['acao'] == 'C' ){
	if($_SESSION["academico"]['entidcampus'] == '' ){
        echo "
            <script>
                alert('Selecione um Campus!');
                window.location = 'academico.php?modulo=principal/listadecampi&acao=A';
            </script>";
        die;
    }
	$entid 	 						 	  = academico_existecampus( $_SESSION["academico"]['entidcampus'] );
	$_SESSION['sig_var']['iscampus'] 	  = 'sim';
	$_SESSION["academico"]["entidcampus"] = $entid;	
	$param	 							  = "&entidcampus=" . $entid;	
	$unidade 							  = $autoriazacaoconcursos->buscaentidade($entid);
	$acao = 'C';	
}



// Verifica a entidade passada, se não existir exibe erro
//$entid = $_REQUEST['entidcampus'] ? $_REQUEST['entidcampus'] : $_REQUEST['entidunidade'];
//$existecampus = academico_existeentidade( $entid );
	
//if( !$existecampus ){
if( !$entid ){
	echo "<script>
			alert('A Unidade informada não existe!');
			history.back(-1);
		  </script>";
	die;
}



// Busca a unidade pai do Campus
//$unidade = $_REQUEST['entidunidade'] ? $entid : $autoriazacaoconcursos->buscaentidade($entid);

// Cabeçalho padrão do simec
require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

// Monta as abas
if( $_SESSION['academico']['entidadenivel'] == 'campus' ){
	if ($_SESSION['baselogin'] == "simec_desenvolvimento"){
		$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57277' : '57136'; // Dados Desenv
	}else{
		$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57268' : '57136';
	}
}else{
	if ($_SESSION['baselogin'] == "simec_desenvolvimento"){
		$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57276' : '57135'; //Dados Desenv
	}else{
		$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57267' : '57135';
	}
}
#$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( "Obras", "");

// mensagem de bloqueio
$bloqueado = academico_mensagem_bloqueio($_SESSION['academico']['orgid']);

$perfilNotBloq = array(PERFIL_IFESCADBOLSAS, 
					   PERFIL_IFESCADCURSOS, 
					   PERFIL_IFESCADASTRO, 
					   PERFIL_MECCADBOLSAS,
					   PERFIL_MECCADCURSOS,
					   PERFIL_MECCADASTRO,
					   PERFIL_ADMINISTRADOR);
// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);
validaAcessoTipoEnsino($permissoes['vertipoensino'],$_SESSION['academico']['orgid']);
// Fim segurança
$bloqueado = (!$bloqueado && $permissoes['gravar']) ? false : true;


?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./geral/js/academico.js"></script>
<div class='col-md-2' style='position:static;'>
    <?=cria_aba_2($abacod_tela, $url, $parametros);?>
</div>
<div class='col-md-9' style='position:static;'>
<?php
// Monta o cabelaçho padrão do módulo
echo $autoriazacaoconcursos->cabecalho_entidade($entid);
?>
<form action="academico.php?modulo=principal/lista_de_obras&acao=A&exec_function=salvarObrasInaugurada" method="post" name="formulario" id="formulario">
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td class="SubTituloEsquerda">Obra(s) a ser(em) inaugurada(s)</td>
		</tr>
		<tr>
			<td>
			<?php 
				switch( $_SESSION['sig_var']['iscampus'] ){
					case 'nao':
						academico_busca_campus_obras( $unidade, 'inauguradas' );
						$valor_entidade = $unidade;
					break;
					case 'sim':
						academico_busca_obras( $entid, '100%', 'inauguradas' );
						$valor_entidade = $entid;
					break;
				}
				
//				$valor_entidade = $_REQUEST['acao'] == 'A' ? $unidade : $entid;
				
			?>
			</td>
		</tr>
		<tr>
			<?php if( !$bloqueado ) { ?>
				<td class="SubTituloDireita"><input type="button" value="Inserir/Remover obra a ser inaugurada" onclick="academico_inserirobrainaugurada('<?=$_REQUEST['acao'] ?>', <?=$valor_entidade ?>);"></td>
			<?php } ?>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Outra(s) obra(s)</td>
		</tr>
	</table>
</form>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td>
		<?php
		
			switch( $_SESSION['sig_var']['iscampus'] ){
				case 'nao':
					academico_busca_campus_obras( $unidade, 'naoinauguradas');
				break;
				case 'sim':
					academico_busca_obras( $entid, '100%', 'naoinauguradas');
				break;
			}
		?>
				
		</td>
	</tr>
</table>
</div>
<script type="text/javascript">

	var params;
	
	//mantendo a arvore expandida
	<?php if( isset($_REQUEST["carga"]) ){ ?>
		abreconteudo('academico.php?modulo=principal/lista_de_obras&acao=A&subAcao=gravarCarga&carga=<?=$_REQUEST["carga"]?>&params=' + params, <?=$_REQUEST["carga"]?>);
	<?php } ?>	
	
	function formatarParametros(){
	    //params = Form.serialize($('pesquisar'));
	    
	}
	
	function desabilitarConteudo( id ){
		var url = 'academico.php?modulo=principal/lista_de_obras&acao=A&carga='+id;
		if ( document.getElementById('img'+id).name == '-' ) {
			url = url + '&subAcao=retirarCarga';
			var myAjax = new Ajax.Request(
				url,
				{
					method: 'post',
					asynchronous: false
				});
		}
	}

	function abreExtrato( obrid ){
		window.open('academico.php?modulo=principal/extrato_obra&acao=A&obrid=' + obrid,'teste', 'scrollbars=yes,width=700,height=500')
	}

</script>

