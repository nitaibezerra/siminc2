<?php include APPRAIZ . 'includes/cabecalho.inc'; echo '</br>'; ?>
<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">	
	<tr>
		<td style="font-size: 18px;">

	    	<p>Prezados usuários,</p>
	    	<p>&nbsp;</p><br/>
			<p style="line-height: 180%;">Buscando sempre melhorar nossos serviços e manter atualizada nossa estrutura de equipamentos, 
			comunicamos que, a partir de hoje, estaremos migrando nosso sistema para EBSERH - Empresa Brasileira de Serviços Hospitalares. 
			O novo acesso ocorrerá através do endereço <a href="http://sig.ebserh.gov.br" target="_blank">sig.ebserh.gov.br</a>, e todas as informações dos usuários, como senhas e acessos, serão mantidas. 
			Em caso de dúvida, favor entrar em contato no 
			e-mail: <a href="mailto:<?php echo $_SESSION['email_sistema']; ?>"><?php echo $_SESSION['email_sistema']; ?></a>.</p>
			<p>&nbsp;</p><br/>
			<p>Equipe Mais Médicos/SIMEC</p>

		</td>
	</tr>
</table>
<?php 

include_once APPRAIZ. "includes/connection/adapter-connection.php";

/*
# LOCAL, DESENVOLVIMENTO E HOMOLOGAÇÃO - Conexão extra dbSimec
$configDbSig = new stdClass();
$configDbSig->host = '';
$configDbSig->port = ;
$configDbSig->dbname = '';
$configDbSig->user = '';
$configDbSig->password = '';
$configDbSig->clientEncoding = 'LATIN5';
$configDbSig->searchPath = 'SET search_path TO public, seguranca';
*/

$configDbSig = new stdClass();
$configDbSig->host = '';
$configDbSig->port = ;
$configDbSig->dbname = '';
$configDbSig->user = '';
$configDbSig->password = '';
$configDbSig->clientEncoding = 'LATIN5';
$configDbSig->searchPath = 'SET search_path TO public, seguranca';

$sql = "select * from maismedicos.maismedicos limit 10";

$listaResultado = adapterConnection::sig()->carregar($sql);
ver($configDbSig, $listaResultado);

?>
