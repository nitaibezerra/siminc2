<?php
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Cache-control: private, no-cache");
header("Last-Modified: " . gmdate( "D, d M Y H:i:s" ) . " GMT");
header("Pragma: no-cache");

require_once "config.inc";

$cpf = $_SESSION["usucpforigem"];
$oid = $_SESSION["estoid"];

if( (time() - $_SESSION["evHoraUltimoAcesso"]) >= MAXONLINETIME or !$cpf) {
	// código de investigação de problemas
	$remetente     = array("nome"=>SIGLA_SISTEMA. " - Queda de sessão", "email"=>"noreply@mec.gov.br");
	$destinatarios = array($_SESSION['email_sistema']);
	$conteudo .= 'Parametros 1 : t1.'.time().',t2.'.$_SESSION["evHoraUltimoAcesso"].', total.'.(time() - $_SESSION["evHoraUltimoAcesso"]).', t3.'.MAXONLINETIME.', condição.'.((time() - $_SESSION["evHoraUltimoAcesso"]) >= MAXONLINETIME).'<br>';
	$conteudo .= 'Parametros 2 : cpf.'.$cpf.'<br>';
	ob_start();
	echo "<pre>";
	print_r($_SERVER);
	print_r($_SESSION);
	$conteudo .= ob_get_contents();
	ob_clean();
	simec_email( $remetente, $destinatarios, 'Quada de sessão', $conteudo);
	// fim - código de investigação de problemas
	session_unset();
	die("EXIT");
}
session_write_close();

$conn = pg_connect( "host=$servidor_bd port=$porta_bd dbname=$nome_bd user=$usuario_db password=$senha_bd" ) or die('Falha na conexão');
//pg_query( $conn, "SET search_path TO seguranca,monitora,elabrev,public" );
pg_set_client_encoding( $conn, 'LATIN5' );

// VERIFICA QUAL OPERAÇÂO REALIZAR
switch( $_REQUEST["op"] )
{
	case "apagar":
		error_reporting( E_ALL );
		ini_set( 'display_error', 1 );
		header( 'Content-Type: text/plain; charset=iso-8859-1' );
		$ids = trim( $_REQUEST["msglist"] );
		if ( $ids{0} == ',' )
		{
			echo 'ok';
			break;
		}
		$sqlApagar = "DELETE FROM seguranca.mensagemchat
			WHERE msgid IN ($ids)
			AND usucpfdestino = '{$_SESSION["usucpforigem"]}'";
		pg_query($conn, "BEGIN");
		$rs = pg_query($conn, $sqlApagar);
		pg_query($conn, "COMMIT");
		echo pg_affected_rows( $rs ) > 0 ? 'ok' : 'erro';
	break;
	case "enviar":
		$_SESSION["evHoraUltimoAcesso"] = time();
		header('Content-Type: text/plain; charset=iso-8859-1');
		pg_query($conn, "BEGIN");
		$mensagem = substr(str_replace("]]>", "", trim(strip_tags($_REQUEST["msg"]))),0,500);
		//Insere Mensagens no Banco
		$sqlInserir = "INSERT INTO seguranca.mensagemchat (usucpforigem, usucpfdestino, msgdsc) VALUES ('%s', '%s', '%s')";
		$sql = sprintf($sqlInserir, $cpf, $_REQUEST["usucpfdestino"], $mensagem);
		$rs = pg_query($conn, $sql);
		$sqlHistorico = "INSERT INTO seguranca.mensagemchathistorico (usucpforigem, usucpfdestino, msgdsc) VALUES ('%s', '%s', '%s')";
		$sql = sprintf($sqlHistorico, $cpf, $_REQUEST["usucpfdestino"], $mensagem);
		$rs = pg_query($conn, $sql);
		pg_query($conn, "COMMIT");
		echo pg_affected_rows( $rs ) > 0 ? 'ok' : 'erro';
	break;
	default:
		header('Content-Type: text/xml; charset=iso-8859-1');
		//Pega as mensagens ainda não lidas
		$sqlChat = "SELECT mc.msgid, mc.usucpforigem , mc.msgdsc, u.usunome, " .
				" TO_CHAR(msgdtenviada, 'DD/MM/YYYY') AS data," . 
				" TO_CHAR(msgdtenviada, 'HH24:MI:SS') AS hora" .
				" FROM seguranca.mensagemchat mc " .
				" join seguranca.usuario u on ( u.usucpf = mc.usucpforigem ) " .
				" WHERE usucpfdestino = '" . $cpf . "' " . 
				" ORDER BY mc.usucpforigem, mc.msgdtenviada";
		$rs = pg_query( $conn, $sqlChat );	
		echo "<evChat usuariosOnLine=\"" . $_SESSION['qtdusuariosonline'][$_SESSION['sisid']] . "\">\n";
		if( @pg_num_rows($rs) > 0 )
		{
			echo "<arrayOfMensagens>\n";
			while( $linha = pg_fetch_assoc( $rs ) )
			{
				$nomeRemetente = explode( " ", $linha["usunome"] );
				$nomeRemetente = ucfirst( strtolower( $nomeRemetente[0] ) );
				echo "<mensagem id=\"" . $linha["msgid"] . "\" data=\"" . $linha["data"] . "\" hora=\"" . $linha["hora"] . "\" remetente=\"" . $linha["usucpforigem"] . "\" nome=\"" . $nomeRemetente . "\">";
					echo "<![CDATA[" . simec_htmlentities(rawurldecode($linha["msgdsc"])) . "]]>";
				echo "</mensagem>\n";
			}
			echo "</arrayOfMensagens>\n";
		}
		echo "</evChat>";
	break;
}
@pg_close($conn);
echo "\n";
?>