<?php

// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

# Verifica se a sessão não expirou, se tiver expirada envia pra tela de login.
controlarAcessoSemAutenticacao();

$db = new cls_banco();

header( 'Content-Type: text/plain; charset=iso-8859-1' );
//header( 'Content-Type: text/html; charset=utf-8' );

$sql_base = <<<EOT
insert into public.naturezareceita (
	ctecod, oricod, espcod, rbrcod, alncod, salcod, nrccod, nrcstatus, nrcano, nrcdsc
) values (
	 %d, %d, %d, '%s', '%s', '%s', '%s', '%s', '%s',
	'%s'
);
EOT;

$linhas = file( 'naturezadereceita.txt' );
foreach ( $linhas as $linha )
{
	$ctecod = substr( $linha, 0, 1 );
	$oricod = substr( $linha, 1, 1 );
	$espcod = substr( $linha, 2, 1 );
	$rbrcod = substr( $linha, 3, 1 );
	$alncod = substr( $linha, 5, 2 );
	$salcod = substr( $linha, 8, 2 );
	$nrccod = $ctecod . $oricod . $espcod . $rbrcod . $alncod . $salcod;
	$nrcstatus = strpos( $linha, ')(E)' ) ? 'I' : 'A';
	$nrcano = '2007';
	$nrcdsc = substr( $linha, 11 );
	$pos = strpos( $nrcdsc, '(' );
	if ( $pos )
	{
		$nrcdsc = substr( $nrcdsc, 0, $pos ) . "\n";
	}
	$nrcdsc = trim( $nrcdsc );
	$sql = sprintf(
		$sql_base,
			$ctecod,	$oricod,	$espcod,	$rbrcod,
			$alncod,	$salcod,	$nrccod,	$nrcstatus,
			$nrcano,	$nrcdsc
	);
	print $sql . "\n\n";
	//$db->executar( $sql );
}
//$db->commit();

