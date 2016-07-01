<?php
// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );   
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

require_once APPRAIZ . "includes/classes/entidades.class.inc";

if ($_REQUEST['opt'] == 'salvarRegistro') {
	
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
	$entidade->salvar();
	
		
	// Verifica se é atualização de responsáveis
	if($_REQUEST["tr"]){
		echo '<script>
				var coluna = window.opener.document.getElementById("nomeEntidade_'.$entidade->getEntid().'");
				coluna.innerHTML = "'.$_REQUEST['entnome'].'";
				window.close();
			  </script>';
		
	} else {
		// Valida se tentar inserir um responsável já adiocionado
		echo '<script type="text/javascript" src="../includes/prototype.js"></script>
	          <script type="text/javascript">
	          	resp = window.opener.document.getElementById(\'linha_'.$entidade->getEntid().'\');
	        	if (resp){
	        		alert("Este responsável já foi adicionado!");
	        		window.close();
	        	}
	          </script>';
		
        $dadoscontato = $db->carregar("SELECT tprcid, tprcdesc FROM obras.tiporespcontato ORDER BY tprcdesc");
        if($dadoscontato[0]) {
	        $combo_responsabilidade = '<select style="width:170px" class="CampoEstilo" name="tprcid['.$entidade->getEntid().']" id="tprcid['.$entidade->getEntid().']">';
			foreach($dadoscontato as $tc) {
				$combo_responsabilidade .= '<option value="'.$tc['tprcid'].'">'.$tc['tprcdesc'].'</option>';
			}
			$combo_responsabilidade .= '</select>';
        }
        $botoes = '<input type="hidden" name="entid" value="'.$entidade->getEntid().'"/><img src="/imagens/alterar.gif" style="cursor: pointer;" border="0" title="Editar" class="alterar" id="'.$entidade->getEntid().'"/>&nbsp&nbsp&nbsp<img src="/imagens/excluir.gif" style="cursor: pointer"  border="0" title="Excluir" class="excluir" id="'.$entidade->getEntid().'"/>';
		//echo

		$bla = 		'<script type="text/javascript">
					var entidades = window.opener.document.getElementById("entidades");
					entidades.value += \','.$entidade->getEntid().'\';
					var div_inserirResp = window.opener.document.getElementById("tb_inserirResp");
					div_inserirResp.style.display = "none";
					var tabela = window.opener.document.getElementById("responsaveis");
					var tamanho = tabela.rows.length;
					var tr = tabela.insertRow(tamanho);	
					tr.id = "linha_'.$entidade->getEntid().'";
					var colAtivo = tr.insertCell(0);
					var colAcao  = tr.insertCell(1);
					var colCpf   = tr.insertCell(2);
					var colNome  = tr.insertCell(3);
					colAtivo.innerHTML = "<center><input type=radio checked name=rdProponente id=rdProponente value='.$entidade->getEntid().' /></center>"
					colAcao.style.textAlign = "center";
					colAcao.innerHTML = \''.$botoes.'\';
					colCpf.innerHTML = "<center>'.$_REQUEST['entnumcpfcnpj'].'</center>"; 
					colNome.id = "nomeEntidade_'.$entidade->getEntid().'";
					colNome.innerHTML = \'<center>'.$_REQUEST['entnome'].'</center>\';
					window.close();
				</script>';
		//ver($bla,d);
		echo $bla;
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
    <script type="text/javascript" src="../includes/entidades.js"></script>
    <script type="text/javascript" src="/includes/estouvivo.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <script type="text/javascript">
      this._closeWindows = false;
    </script>
  </head>
  <body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
    <div>
<?php
$entidade = new Entidades();
if($_REQUEST['entid'])
	$entidade->carregarPorEntid($_REQUEST['entid']);
	echo $entidade->formEntidade("elabrev.php?modulo=principal/termoCooperacao/inserir_representante&acao=A&opt=salvarRegistro&tr=".$_REQUEST['tr'],
								 array("funid" => RESPONSAVEL_TERMO_COOPERACAO, "entidassociado" => false),
								 array("enderecos"=>array(1))
								 );
?>
    </div>
    <script type="text/javascript">
    
	document.getElementById('frmEntidade').onsubmit  = function(e) {
	if (document.getElementById('entnumcpfcnpj').value == '') {
		alert('O CPF é obrigatório.');
		return false;
	}

	if (document.getElementById('entnome').value == '') {
		alert('O nome da entidade é obrigatório.');
		return false;
	}
	return true;
	}
	<?if( $_REQUEST['habilita'] == 'N' ){ ?>
	var form = document.getElementById('frmEntidade');
	for(i=0; i<form.length; i++){
		if(form.elements[i].type == 'text' || form.elements[i].type == 'textarea' || form.elements[i].type == 'select-one' || 
					form.elements[i].type == 'button' || form.elements[i].type == 'radio' || form.elements[i].type == 'submit'){
			form.elements[i].disabled = 'disabled';
		}
	}
	<?} ?>
    </script>
  </body>
</html>
