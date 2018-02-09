<?php
header( 'Content-Type: text/html; charset=ISO-8859-1' );
//header( 'Content-Type: text/html; charset=UTF-8' );

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );


error_reporting( E_ALL ^ E_NOTICE );

ini_set("memory_limit", "1024M");
set_time_limit(0);

ini_set( 'soap.wsdl_cache_enabled', '0' );
ini_set( 'soap.wsdl_cache_ttl', 0 );


$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . "www/sispacto/_constantes.php";
require_once APPRAIZ . "www/sispacto/_funcoes.php";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
    
   
// abre conexção com o servidor de banco de dados
$db = new cls_banco();

$sql = "update sispacto.identificacaousuario set iusemailprincipal=replace(iusemailprincipal,'@.','@') where iusemailprincipal ilike '%@.%';";
$db->executar($sql);
$db->commit();

$sql = "select distinct i.iusd, i.iuscpf, p.pfldsc, u.uninome, logrequest, s.logresponse, case when pp.muncod is not null then m.estuf || ' / ' || m.mundescricao 
																							   when pp.estuf is not null then es.estuf || ' / ' || es.estdescricao
																							   else 'equipe ies' end as esfera

from sispacto.identificacaousuario i 
inner join sispacto.tipoperfil t on t.iusd = i.iusd 
inner join seguranca.perfil p on p.pflcod = t.pflcod 
left join sispacto.universidadecadastro c on c.uncid = i.uncid 
left join sispacto.universidade u on u.uniid = c.uniid 
left join sispacto.pactoidadecerta pp on pp.picid = i.picid 
left join territorios.municipio m on m.muncod = pp.muncod 
left join territorios.estado es on es.estuf = pp.estuf
inner join sispacto.logsgb s on s.logcpf = i.iuscpf and s.logservico='gravarDadosBolsista' and s.logerro=true
where iustermocompromisso=true and logresponse ilike '%Erro: 00026:%';";

$arr = $db->carregar($sql);

echo '<table>';
$i=0;
if($arr[0]) {

	foreach($arr as $ar) {
		
		$sx = explode("<no_pessoa xsi:type=\"xsd:string\">",$ar['logrequest']);
		$sx = explode("</no_pessoa>",$sx[1]);
		
		$sl = explode("(",$ar['logresponse']);
		$sl = explode(")",$sl[1]);
		
		if(substr(strtoupper(trim($sx[0])),0,9)!=substr(strtoupper(trim($sl[0])),0,9)) {
			echo '<tr>';
			echo '<td>'.$ar['iuscpf'].'</td>';
			echo '<td>'.$sx[0].'</td>';
			echo '<td>'.$sl[0].'</td>';
			echo '<td>'.$ar['pfldsc'].'</td>';
			echo '<td>'.$ar['esfera'].'</td>';
			echo '<td>'.$ar['uninome'].'</td>';
			echo '</tr>';
			$i++;
		}

	}
}
echo '</table>';

echo "Total : ".$i;

?>