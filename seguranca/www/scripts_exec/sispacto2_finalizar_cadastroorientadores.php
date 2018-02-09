<?php

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);} 

date_default_timezone_set ('America/Sao_Paulo');

$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

// carrega as funções gerais
//include_once "/var/www/simec/global/config.inc";
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/workflow.php";
include_once APPRAIZ . "www/sispacto2/_constantes.php";
include_once APPRAIZ . "www/sispacto2/_funcoes.php";
include_once APPRAIZ . "www/sispacto2/_funcoes_coordenadorlocal.php";

$_SESSION['sisid']        = '181';

// CPF do administrador de sistemas
if(!$_SESSION['usucpf']) {
	$_SESSION['usucpf']       = '00000000191';
	$_SESSION['usucpforigem'] = '00000000191';
}


// abre conexão com o servidor de banco de dados
$db = new cls_banco();


///////////////////////////////////////////////////////////
$pactoidadecerta_naoiniciou = $db->carregar("SELECT p.* FROM sispacto2.pactoidadecerta p 
					    		  		   	 WHERE docid IS NULL");

if($pactoidadecerta_naoiniciou[0]) {
	foreach($pactoidadecerta_naoiniciou as $p) {
		$docid = wf_cadastrarDocumento(TPD_ORIENTADORESTUDO,"Sispacto_CoordenadorLocal_".(($p['estuf'])?"estuf_".$p['estuf']."_":"").(($p['muncod'])?"muncod_".$p['muncod']:""));
		$db->executar("UPDATE sispacto2.pactoidadecerta SET docid='".$docid."', picselecaopublica=true WHERE picid='".$p['picid']."'");
		$db->commit();
		echo 'Fluxo criado com sucesso : '.$p['picid'].'<br>';
	}
}

///////////////////////////////////////////////////////////
$pactoidadecerta_emelaboracao = $db->carregar("SELECT p.picid, 
													  p.muncod, 
													  p.estuf, 
													  CASE WHEN m.muncod IS NOT NULL THEN m.estuf||' / '||m.mundescricao ELSE e.estuf||' / '||e.estdescricao END as descricao
											   FROM sispacto2.pactoidadecerta p 
					    		  		   	   INNER JOIN workflow.documento d ON d.docid = p.docid 
					    		  		   	   LEFT JOIN territorios.municipio m ON m.muncod = p.muncod 
					    		  		   	   LEFT JOIN territorios.estado e ON e.estuf = p.estuf 
					    		  		   	   WHERE d.esdid='".ESD_ELABORACAO_COORDENADOR_LOCAL."'");


if($pactoidadecerta_emelaboracao[0]) {
	foreach($pactoidadecerta_emelaboracao as $p) {
		
		$ar = array("estuf" 	  => $p['estuf'],
					"muncod" 	  => $p['muncod'],
					"dependencia" => (($p['muncod'])?'municipal':'estadual'));
		
		$totalalfabetizadores = carregarTotalAlfabetizadores($ar);
		
		$orientadoresestudo = carregarDadosIdentificacaoUsuario(array("picid"=>$p['picid'],"pflcod"=>PFL_ORIENTADORESTUDO));
		
		if($totalalfabetizadores['total_orientadores_a_serem_cadastrados'] > count($orientadoresestudo)) {
			$restantes = ($totalalfabetizadores['total_orientadores_a_serem_cadastrados']-count($orientadoresestudo));
			for($i = 0;$i < $restantes;$i++) {
				
				$num_ius = $db->pegaUm("SELECT substr(iuscpf, 8) as num FROM sispacto2.identificacaousuario WHERE picid='".$p['picid']."' AND iuscpf ilike 'SIS%' ORDER BY iusd DESC");
				if($num_ius) $num_ius++;
				else $num_ius=1;
				
				$iuscpf  		   = "SIS".str_pad($p['picid'], 4, "0", STR_PAD_LEFT).str_pad($num_ius, 4, "0", STR_PAD_LEFT);
				$iusnome 		   = "Orientador de Estudo - ".str_replace("'"," ",$p['descricao'])." - ".str_pad($num_ius, 4, "0", STR_PAD_LEFT);
				$iusemailprincipal = "noemail@noemail.com"; 
				
				$sql = "INSERT INTO sispacto2.identificacaousuario(picid, 
																  muncod, 
																  iuscpf, 
																  iusnome, 
            													  iusemailprincipal, 
            													  iustipoorientador, 
            													  muncodatuacao,
            													  iusdatainclusao )
					    VALUES ('".$p['picid']."', 
					    		".(($p['muncod'])?"'".$p['muncod']."'":"NULL").", 
					    		'".$iuscpf."', 
					    		'".$iusnome."', 
					    		'".$iusemailprincipal."', 
					    		'profissionaismagisterio', 
					    		".(($p['muncod'])?"'".$p['muncod']."'":"NULL").",
					    		NOW()) RETURNING iusd;";
				
				$iusd = $db->pegaUm($sql);
				
				$sql = "INSERT INTO sispacto2.tipoperfil( iusd, pflcod, tpestatus)
    					VALUES ( '".$iusd."', '".PFL_ORIENTADORESTUDO."', 'A');";
				
				$db->executar($sql);
				
			}
			
			$db->commit();
		}

	}
}

///////////////////////////////////////////////////////////
$pactoidadecerta_emelaboracao = $db->carregar("SELECT p.*
											   FROM sispacto2.pactoidadecerta p 
					    		  		   	   INNER JOIN workflow.documento d ON d.docid = p.docid 
					    		  		   	   LEFT JOIN territorios.municipio m ON m.muncod = p.muncod 
					    		  		   	   LEFT JOIN territorios.estado e ON e.estuf = p.estuf 
					    		  		   	   WHERE d.esdid='".ESD_ELABORACAO_COORDENADOR_LOCAL."'");

if($pactoidadecerta_emelaboracao[0]) {
	foreach($pactoidadecerta_emelaboracao as $p) {
		$_SESSION['sispacto2']['coordenadorlocal']['naoValidarEnvioAnaliseIES'] = true;
		$result = wf_alterarEstado( $p['docid'], 2294, null, array() );
		echo 'Enviado para análise da IES : '.$result.'<br>';
	}
}

$db->close();

echo "fim";

?>