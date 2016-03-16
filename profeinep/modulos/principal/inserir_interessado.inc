<?php

require_once APPRAIZ . "includes/classes/entidades.class.inc";

// inserindo entidade e inserindo na tabela da página pai (via javascript)
if ($_REQUEST['opt'] == 'salvarRegistro') {
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
	$entidade->salvar();
	// inserindo no formulario pai (opener)
    $codigo = '<input type="hidden" value="'.$entidade->getEntid().'" id="entid_'.$entidade->getEntid().'" name="entid[]"/>';
	$id     = '<input type="hidden" value="0" name="recoid_0" />';
    $botoes = '<img src="/imagens/alterar.gif" style="cursor: pointer" border="0" title="Editar" onclick="atualizaInteressado(\\\'linha_' . $entidade->getEntid(). '\\\');"/>&nbsp<img src="/imagens/excluir.gif" style="cursor: pointer"  border="0" title="Excluir" onclick="RemoveLinha(window.document.getElementById(\\\'linha_'.$entidade->getEntid().'\\\').rowIndex);"/>'; 
    if(!$_REQUEST['tr'] || $_REQUEST['tr'] == ''){
    	echo '<script type="text/javascript">
			
				var atual = window.opener.document.getElementById("linha_'.$entidade->getEntid().'"); 
				var tabela = window.opener.document.getElementById("tabela_interessado");
				if (atual){
    				tabela.deleteRow(atual.rowIndex);
    			}
				var tamanho = tabela.rows.length;
				
				if(window.opener.document.getElementById("linha_'.$entidade->getEntid().'") == null){
					var tr = tabela.insertRow(tamanho);	
					tr.id = "linha_'.$entidade->getEntid().'";
				
					var colAcao = tr.insertCell(0);
					var colNome = tr.insertCell(1);
					var colResp = tr.insertCell(2);
					
					colAcao.style.textAlign = "center";
					
					colAcao.innerHTML = \''.$botoes.'\';
					colNome.innerHTML = \'' . $codigo . $_REQUEST['entnome'] . $id . '\';
					colResp.innerHTML = \'' . $_REQUEST['tpPessoa'] . '\'; 
				}
				window.close();

			</script>';
    	
	} else {
    	echo '<script type="text/javascript">
				window.close();
			  </script>';
	}
	exit;

}


?>
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
    <title><?= $titulo ?></title>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/prototype.js"></script>
    

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <script type="text/javascript">
      this._closeWindows = false;
    </script>
  </head>
  <body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
    <div>
<?

if($_REQUEST['destino']) {
	$_SESSION['profeinep_var']['destino'] = $_REQUEST['destino'];
}

$entidade = new Entidades();

if($_REQUEST['entid']) {
	$entidade->carregarPorEntid($_REQUEST['entid']);
}
echo $entidade->formEntidade("profeinep.php?modulo=principal/inserir_interessado&acao=A&opt=salvarRegistro&tpPessoa=Física&destino=".$_SESSION['profeinep_var']['destino'],
							 array("funid" => FUN_PARTICIPANTEPROFEINEP, "entidassociado" => null),
							 array("enderecos"=>array(1))
							 );
?>
    </div>
</body>
</html>