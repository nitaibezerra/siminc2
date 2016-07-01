<?php
include_once "config.inc";
include "verificasistema.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();


require_once(APPRAIZ . 'altorc/classes/WSAlteracoesOrcamentarias.php');

$servico = new WSAlteracoesOrcamentarias();
ver($servico->obterEmendasAprovadas());




//include 'config.inc';
//
//$filename = APPRAIZ . 'spo/certificados/serpro_siafi.ini';
//$a = parse_ini_file($filename, true);
//
//echo '<pre>';
//var_dump($a);


//$sql = <<<DML
//SELECT DISTINCT aca.acacod AS codigo,
//                aca.acacod || ' - ' || aca.acadsc AS descricao
//  FROM elabrev.ppaacao_orcamento aca
//  WHERE aca.prgano = '{$exercicio}'
//    AND aca.acasnrap = 'f'
//    AND EXISTS (SELECT 1
//                  FROM proporc.despesaacao dpa
//                  WHERE aca.acacod = dpa.acacod
//                    AND dpa.dspid = %d)
//  ORDER BY acacod
//DML;
//
//echo '<pre>';
//$retorno = array();
//$str = substr($sql, 0, strpos($sql, 'WHERE'));
//
//
//preg_match('/SELECT .* (WHERE+)/', strtoupper($sql), $retorno);
//
//var_dump($sql, $retorno, d);



//require_once(APPRAIZ . '/includes/Aes/aes.class');
//require_once(APPRAIZ . '/includes/Aes/aesctr.class');
//
//require_once(APPRAIZ. '/www/progfin/_funcoes.php');
//
//var_dump(AES256_CBC_dec(AES256_CBC_enc('joao-ninguem')));

//
//$segredo_AES = 'lekyam6963';
//var_dump($segredo_AES);
//$segredo_AES = AesCtr::encrypt($segredo_AES, 'S1M3C__PaSs_PrOG4M4c40fIn4NC31r4', 256);
//var_dump($segredo_AES);
//$segredo_AES = base64_encode($segredo_AES);
//var_dump($segredo_AES);
//echo '---------- AES ---------<br />';
//$cifra_AES = 'Y1Axd29RNjVMVlFhVk5VVElxdTNkQTZD';
//var_dump($cifra_AES);
//$cifra_AES = base64_decode($cifra_AES);
//var_dump($cifra_AES);
//$cifra_AES = AesCtr::decrypt($cifra_AES, 'S1M3C__PaSs_PrOG4M4c40fIn4NC31r4', 256);
//var_dump($cifra_AES);
//echo '-------------------<br />';
//echo '-------------------<br />';
////$ivsize = mcrypt_get_iv_size(MCRYPT_RIJNDAEL_128, MCRYPT_MODE_CBC);
////$iv = mcrypt_create_iv($ivsize, MCRYPT_RAND);
////$segredo_mcrypt = 'maykelsantosbraz';
////var_dump($segredo_mcrypt);
////$segredo_mcrypt = $iv . mcrypt_encrypt(
////    MCRYPT_RIJNDAEL_128,
////    'S1M3C__PaSs_PrOG4M4c40fIn4NC31r4',
////    $segredo_mcrypt,
////    MCRYPT_MODE_CBC,
////    $iv
////);
////var_dump($segredo_mcrypt);
////$segredo_mcrypt = base64_encode($segredo_mcrypt);
////var_dump($segredo_mcrypt);
////echo '---------- MCRYPT ---------<br />';
////$cifra_mcrypt = 'ZnYxeXdTRzRMVlJ0RWJuaTcxYmVvMDNnUFE9PQ==';
////var_dump($cifra_mcrypt);
////$cifra_mcrypt = base64_decode($cifra_mcrypt);
////var_dump($cifra_mcrypt);
////$cifra_mcrypt = mcrypt_decrypt(
////    MCRYPT_RIJNDAEL_128,
////    'S1M3C__PaSs_PrOG4M4c40fIn4NC31r4',
////    substr($cifra_mcrypt, $ivsize),
////    MCRYPT_MODE_CBC,
////    substr($cifra_mcrypt, 0, $ivsize)
////);
////var_dump($cifra_mcrypt);
////echo '-------------------<br />';
////echo '-------------------<br />';
////$cifra_mcrypt = $segredo_AES;
////var_dump($cifra_mcrypt);
////$cifra_mcrypt = base64_decode($cifra_mcrypt);
////var_dump($cifra_mcrypt);
////$cifra_mcrypt = mcrypt_decrypt(
////    MCRYPT_RIJNDAEL_128,
////    'S1M3C__PaSs_PrOG4M4c40fIn4NC31r4',
////    substr($cifra_mcrypt, $ivsize),
////    MCRYPT_MODE_CBC,
////    substr($cifra_mcrypt, 0, $ivsize)
////);
////var_dump($cifra_mcrypt);
