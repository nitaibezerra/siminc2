<?php
ini_set("memory_limit", "2048M");
set_time_limit(30000); 

include  APPRAIZ."includes/cabecalho.inc";

include_once APPRAIZ."emenda/classes/WSAlteracoesOrcamentariasEmendas.class.inc";
echo '22';
/* $usuario = '73282081120';
$senha = 'Simec01#';  */


?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form action="" method="post" id="formulario" name="formulario">
<input type="hidden" name="requisicao" id="requisicao" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=6 align="center" style="width: 100%">
	<tr>
		<th colspan="4" style="text-align: center;">Cliente WS</th>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="25%">Diretório Certificado:</td>
		<td colspan="3"><?php
			$caminho = ($_POST['caminho'] ? $_POST['caminho'] : $caminho_certificado);
			echo campo_texto('caminho', 'N', 'S', '', 150, 200, '', '');
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Senha Certificado:</td>
		<td colspan="3">
		<?php 
			$senha = ( $_POST['senha'] ? $_POST['senha'] : $senha_certificado);
			echo campo_texto( 'senha', 'N', 'S', '', 50, 200, '', ''); 
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >URL:</td>
		<td colspan="3">
		<?php
		$wsdl = ( $_POST['wsdl'] ? $_POST['wsdl'] : $wsdl);
		echo campo_texto( 'wsdl', 'N', 'S', '', 150, 200, '', '');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan="4" style="text-align: center;">
			<input class="botao" type="button" name="enviar" id= "enviar" value="Enviar" onclick="submeteFormulario('1');">
			<input class="botao" type="button" name="visualizar" id= "visualizar" value="Visualizar XML" onclick="submeteFormulario('2');">
		</td>
	</tr>
</table>
</form>
<script type="text/javascript">
function submeteFormulario( tipo ){
	if( tipo == 2 ) $('#requisicao').val('visualizar');
	else $('#requisicao').val('enviar');
	$('#formulario').submit();
}
</script>
<?php 
if( $_REQUEST['requisicao'] == 'enviar' || $_REQUEST['requisicao'] == 'visualizar' ){

$wsdl 					= $_REQUEST['wsdl'];
$caminho_certificado 	= APPRAIZ.$_REQUEST['caminho'];
$senha_certificado 		= $_REQUEST['senha'];

$str = <<<XML
XML;

ini_set('soap.wsdl_cache_enabled', 0);
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=6 align="center" style="width: 100%">
<tr>
	<th colspan="10">Resultado da Solicitação</th>
</tr>
<?
if( $_REQUEST['requisicao'] == 'visualizar' ){
	$xmlDom = execute($wsdl, $caminho_certificado, $senha_certificado);
	?>
	<tr>
		<td colspan="10" style="font-size: 12px;"><pre> wes <? print_r(simec_htmlentities($xmlDom)); ?></pre> </td>
	</tr>
	<?
} else {
	echo '222';
?>
	<tr>
		<td colspan="10" style="font-size: 12px;"><pre> <? 
ini_set('soap.wsdl_cache_enabled', 0);

print "Analyzing WSDL";

try {
	if( $caminho_certificado && $senha_certificado ){	
		  $context = stream_context_create(array(
											    'ssl' => array(
											    'verify_peer' => false,
											    'allow_self_signed' => true,
											    'ciphers'=>"SHA1",
											    'cafile' => $caminho_certificado,
											    'local_cert' => file_get_contents($caminho_certificado),
											    'passphrase ' 	=> $senha_certificado	
											    )
											)); 
	}
	//$wsdl = 'https://wshom.convenios.gov.br/siconv-interfaceConvenio-1.0/InterfaceConvenioHandler/InterfaceConvenioHandlerBeanImpl?wsdl';
	
	$opts = array(
			'ssl'   => array(
					'verify_peer'          => false,
					'allow_self-signed' => true,
					'ciphers' => "SHA1"
			)
	);
	$streamContext = stream_context_create($opts);
	
	$params = array(
			'features' => SOAP_SINGLE_ELEMENT_ARRAYS,
			'stream_context'     => $streamContext,
			'cache_wsdl' => NULL,
	);
	
	$client = new SoapClient($wsdl, $params);
	/* $client = new SoapClient($wsdl, array(
									'exceptions'	=> true,
							        'trace'			=> true,
									'encoding'		=> 'ISO-8859-1',
									'local_cert'        => file_get_contents($caminho_certificado),
									'passphrase'        => $senha_certificado,
									'stream_context' => $context
									)
								); */
  $operations = $client->__getFunctions();
} catch(SoapFault $e) {
	die($e);
}
ver($operations,d);
print ".";
//ver(simec_htmlentities($xmlDom));
$dom = DOMDocument::loadXML($str);

print ".";
// get documentation
$nodes = $dom->getElementsByTagName('documentation');
$doc = array('service' => '',
	     'operations' => array());
foreach($nodes as $node) {
  if( $node->parentNode->localName == 'service' ) {
    $doc['service'] = trim($node->parentNode->nodeValue);
  } else if( $node->parentNode->localName == 'operation' ) {
    $operation = $node->parentNode->getAttribute('name');
    //$parameterOrder = $node->parentNode->getAttribute('parameterOrder');
    $doc['operations'][$operation] = trim($node->nodeValue);
  }
}
print ".";

// get targetNamespace
$targetNamespace = '';
$nodes = $dom->getElementsByTagName('definitions');
foreach($nodes as $node) {
  $targetNamespace = $node->getAttribute('targetNamespace');
}
print ".";

// declare service
$service = array('class' => 'SimecWsFacadeService',
		 'wsdl' => $wsdl,
		 'doc' => $doc['service'],
		 'functions' => array());
print ".";

// PHP keywords - can not be used as constants, class names or function names!
$reserved_keywords = array('and', 'or', 'xor', 'as', 'break', 'case', 'cfunction', 'class', 'continue', 'declare', 'const', 'default', 'do', 'else', 'elseif', 'enddeclare', 'endfor', 'endforeach', 'endif', 'endswitch', 'endwhile', 'eval', 'extends', 'for', 'foreach', 'function', 'global', 'if', 'new', 'old_function', 'static', 'switch', 'use', 'var', 'while', 'array', 'die', 'echo', 'empty', 'exit', 'include', 'include_once', 'isset', 'list', 'print', 'require', 'require_once', 'return', 'unset', '__file__', '__line__', '__function__', '__class__', 'abstract', 'private', 'public', 'protected', 'throw', 'try');

// ensure legal class name (I don't think using . and whitespaces is allowed in terms of the SOAP standard, should check this out and may throw and exception instead...)
$service['class'] = str_replace(' ', '_', $service['class']);
$service['class'] = str_replace('.', '_', $service['class']);
$service['class'] = str_replace('-', '_', $service['class']);

if(in_array(strtolower($service['class']), $reserved_keywords)) {
  $service['class'] .= 'Service';
}

// verify that the name of the service is named as a defined class
if(class_exists($service['class'])) {
  throw new Exception("Class '".$service['class']."' already exists");
}

/*if(function_exists($service['class'])) {
  throw new Exception("Class '".$service['class']."' can't be used, a function with that name already exists");
}*/

// get operations
$operations = $client->__getFunctions();
foreach($operations as $operation) {

  /*
   This is broken, need to handle
   GetAllByBGName_Response_t GetAllByBGName(string $Name)
   list(int $pcode, string $city, string $area, string $adm_center) GetByBGName(string $Name)

   finding the last '(' should be ok
   */
  //list($call, $params) = explode('(', $operation); // broken
  
  //if($call == 'list') { // a list is returned
  //}
  
  /*$call = array();
  preg_match('/^(list\(.*\)) (.*)\((.*)\)$/', $operation, $call);
  if(sizeof($call) == 3) { // found list()
    
  } else {
    preg_match('/^(.*) (.*)\((.*)\)$/', $operation, $call);
    if(sizeof($call) == 3) {
      
    }
  }*/

  $matches = array();
  if(preg_match('/^(\w[\w\d_]*) (\w[\w\d_]*)\(([\w\$\d,_ ]*)\)$/', $operation, $matches)) {
    $returns = $matches[1];
    $call = $matches[2];
    $params = $matches[3];
  } else if(preg_match('/^(list\([\w\$\d,_ ]*\)) (\w[\w\d_]*)\(([\w\$\d,_ ]*)\)$/', $operation, $matches)) {
    $returns = $matches[1];
    $call = $matches[2];
    $params = $matches[3];
  } else { // invalid function call
    throw new Exception('Invalid function call: '.$function);
  }

  $params = explode(', ', $params);

  $paramsArr = array();
  foreach($params as $param) {
    $paramsArr[] = explode(' ', $param);
  }
  //  $call = explode(' ', $call);
  $function = array('name' => $call,
		    'method' => $call,
		    'return' => $returns,
		    'doc' => isset($doc['operations'][$call])?$doc['operations'][$call]:'',
		    'params' => $paramsArr);

  // ensure legal function name
  if(in_array(strtolower($function['method']), $reserved_keywords)) {
    $function['name'] = '_'.$function['method'];
  }

  // ensure that the method we are adding has not the same name as the constructor
  if(strtolower($service['class']) == strtolower($function['method'])) {
    $function['name'] = '_'.$function['method'];
  }

  // ensure that there's no method that already exists with this name
  // this is most likely a Soap vs HttpGet vs HttpPost problem in WSDL
  // I assume for now that Soap is the one listed first and just skip the rest
  // this should be improved by actually verifying that it's a Soap operation that's in the WSDL file
  // QUICK FIX: just skip function if it already exists
  $add = true;
  foreach($service['functions'] as $func) {
    if($func['name'] == $function['name']) {
      $add = false;
    }
  }
  if($add) {
    $service['functions'][] = $function;
  }
  print ".";
}

$types = $client->__getTypes();

$primitive_types = array('string', 'int', 'long', 'float', 'boolean', 'dateTime', 'double', 'short', 'UNKNOWN', 'base64Binary', 'decimal', 'ArrayOfInt', 'ArrayOfFloat', 'ArrayOfString', 'decimal', 'hexBinary'); // TODO: dateTime is special, maybe use PEAR::Date or similar
$service['types'] = array();
foreach($types as $type) {
  $parts = explode("\n", $type);
  $class = explode(" ", $parts[0]);
  $class = $class[1];
  
  if( substr($class, -2, 2) == '[]' ) { // array skipping
    continue;
  }

  if( substr($class, 0, 7) == 'ArrayOf' ) { // skip 'ArrayOf*' types (from MS.NET, Axis etc.)
    continue;
  }


  $members = array();
  for($i=1; $i<count($parts)-1; $i++) {
    $parts[$i] = trim($parts[$i]);
    list($type, $member) = explode(" ", substr($parts[$i], 0, strlen($parts[$i])-1) );

    // check syntax
    if(preg_match('/^$\w[\w\d_]*$/', $member)) {
      throw new Exception('illegal syntax for member variable: '.$member);
      continue;
    }

    // IMPORTANT: Need to filter out namespace on member if presented
    if(strpos($member, ':')) { // keep the last part
      list($tmp, $member) = explode(':', $member);
    }

    // OBS: Skip member if already presented (this shouldn't happen, but I've actually seen it in a WSDL-file)
    // "It's better to be safe than sorry" (ref Morten Harket) 
    $add = true;
    foreach($members as $mem) {
      if($mem['member'] == $member) {
	$add = false;
      }
    }
    if($add) {
      $members[] = array('member' => $member, 'type' => $type);
    }
  }

  // gather enumeration values
  $values = array();
  if(count($members) == 0) {
    $values = checkForEnum($dom, $class);
  }

  $service['types'][] = array('class' => $class, 'members' => $members, 'values' => $values);
  print ".";
}
print "done\n";

print "Generating code...";
$code = "";

// add types
foreach($service['types'] as $type) {
  //  $code .= "/**\n";
  //  $code .= " * ".(isset($type['doc'])?$type['doc']:'')."\n";
  //  $code .= " * \n";
  //  $code .= " * @package\n";
  //  $code .= " * @copyright\n";
  //  $code .= " */\n";

  // add enumeration values
  $code .= "class ".$type['class']." {\n";
  foreach($type['values'] as $value) {
    $code .= "  const ".generatePHPSymbol($value)." = '$value';\n";
  }
  
  // add member variables
  foreach($type['members'] as $member) {
    //$code .= "  /* ".$member['type']." */\n";
    $code .= "	12public \$".$member['member']."; // ".$member['type']."\n";
  }
  $code .= "}\n\n";

  /*  print "Writing ".$type['class'].".php...";
  $filename = $type['class'].".php";
  $fp = fopen($filename, 'w');
  fwrite($fp, "<?php\n".$code."?>\n");
  fclose($fp);
  print "ok\n";*/
}

// add service

// page level docblock
//$code .= "/**\n";
//$code .= " * ".$service['class']." class file\n";
//$code .= " * \n";
//$code .= " * @author    {author}\n";
//$code .= " * @copyright {copyright}\n";
//$code .= " * @package   {package}\n";
//$code .= " */\n\n";


// require types
//foreach($service['types'] as $type) {
//  $code .= "/**\n";
//  $code .= " * ".$type['class']." class\n";
//  $code .= " */\n";
//  $code .= "require_once '".$type['class'].".php';\n";
//}

$code .= "\n";

// class level docblock
$code .= "/**\n";
$code .= " * ".$service['class']." class\n";
$code .= " * \n";
$code .= parse_doc(" * ", $service['doc']);
$code .= " * \n";
$code .= " * @author    {author}\n";
$code .= " * @copyright {copyright}\n";
$code .= " * @package   {package}\n";
$code .= " */\n";
$code .= "class ".$service['class']." extends SoapClient {\n\n";

// add classmap
$code .= "  private static \$classmap = array(\n";
foreach($service['types'] as $type) {
  $code .= "                                    '".$type['class']."' => '".$type['class']."',\n";
}
$code .= "                                   );\n\n";
$code .= "  public function ".$service['class']."(\$wsdl = \"".$service['wsdl']."\", \$options = array()) {\n";

// initialize classmap (merge)
$code .= "    foreach(self::\$classmap as \$key => \$value) {\n";
$code .= "      if(!isset(\$options['classmap'][\$key])) {\n";
$code .= "        \$options['classmap'][\$key] = \$value;\n";
$code .= "      }\n";
$code .= "    }\n";
$code .= "    parent::__construct(\$wsdl, \$options);\n";
$code .= "  }\n\n";

foreach($service['functions'] as $function) {
  $code .= "  /**\n";
  $code .= parse_doc("   * ", $function['doc']);
  $code .= "   *\n";

  $signature = array(); // used for function signature
  $para = array(); // just variable names
  if(count($function['params']) > 0) {
    foreach($function['params'] as $param) {
      $code .= "   * @param ".(isset($param[0])?$param[0]:'')." ".(isset($param[1])?$param[1]:'')."\n";
      /*$typehint = false;
      foreach($service['types'] as $type) {
	if($type['class'] == $param[0]) {
	  $typehint = true;
	}
      }
      $signature[] = ($typehint) ? implode(' ', $param) : $param[1];*/
      $signature[] = (in_array($param[0], $primitive_types) or substr($param[0], 0, 7) == 'ArrayOf') ? $param[1] : implode(' ', $param);
      $para[] = $param[1];
    }
  }
  $code .= "   * @return ".$function['return']."\n";
  $code .= "   */\n";
  $code .= "  public function ".$function['name']."(".implode(', ', $signature).") {\n";
  //  $code .= "    return \$this->client->".$function['name']."(".implode(', ', $para).");\n";
  $code .= "    return \$this->__soapCall('".$function['method']."', array(";
  $params = array();
  if(count($signature) > 0) { // add arguments
    foreach($signature as $param) {
      if(strpos($param, ' ')) { // slice 
	$param = array_pop(explode(' ', $param));
      }
      $params[] = $param;
    }
    //$code .= "\n      ";
    $code .= implode(", ", $params);
    //$code .= "\n      ),\n";
  }
  $code .= "), ";
  //$code .= implode(', ', $signature)."),\n";
  $code .= "      array(\n";
  $code .= "            'uri' => '".$targetNamespace."',\n";
  $code .= "            'soapaction' => ''\n";
  $code .= "           )\n";
  $code .= "      );\n";
  $code .= "  }\n\n";
}
$code .= "}\n\n";
print "done\n";

//print "Writing ".$service['class'].".php...";
//$fp = fopen(APPRAIZ."/".$service['class'].".php", 'w');
//fwrite($fp, "<?php\n".$code."\n");
//fclose($fp);
echo "<pre>";
echo $code;
print "done\n";
	echo '</pre> </td> </tr>';
}
echo '</table>';
}

function parse_doc($prefix, $doc) {
  $code = "";
  $words = @split(' ', $doc);
  $line = $prefix;
  foreach($words as $word) {
    $line .= $word.' ';
    if( strlen($line) > 90 ) { // new line
      $code .= $line."\n";
      $line = $prefix;
    }
  }
  $code .= $line."\n";
  return $code;
}

/**
 * Look for enumeration
 * 
 * @param DOM $dom
 * @param string $class
 * @return array
 */
function checkForEnum(&$dom, $class) {
  $values = array();
  
  $node = findType($dom, $class);
  if(!$node) {
    return $values;
  }
  
  $value_list = $node->getElementsByTagName('enumeration');
  if($value_list->length == 0) {
    return $values;
  }

  for($i=0; $i<$value_list->length; $i++) {
    $values[] = $value_list->item($i)->attributes->getNamedItem('value')->nodeValue;
  }
  return $values;
}

/**
 * Look for a type
 * 
 * @param DOM $dom
 * @param string $class
 * @return DOMNode
 */
function findType(&$dom, $class) {
  $types_node  = $dom->getElementsByTagName('types')->item(0);
  $schema_list = $types_node->getElementsByTagName('schema');
  
  for ($i=0; $i<$schema_list->length; $i++) {
    $children = $schema_list->item($i)->childNodes;
    for ($j=0; $j<$children->length; $j++) {
      $node = $children->item($j);
      if ($node instanceof DOMElement &&
	  $node->hasAttributes() &&
	  $node->attributes->getNamedItem('name')->nodeValue == $class) {
	return $node;
      }
    }
  }
  return null;
}

function generatePHPSymbol($s) {
  global $reserved_keywords;
  
  if(!preg_match('/^[A-Za-z_]/', $s)) {
    $s = 'value_'.$s;
  }
  if(in_array(strtolower($s), $reserved_keywords)) {
    $s = '_'.$s;
  }
  return preg_replace('/[-.\s]/', '_', $s);
}

function execute($url, $caminho_certificado, $senha_certificado) {
	//echo $url;
	
	// Initialize session and set URL.
	$ch = curl_init();
	curl_setopt($ch, CURLOPT_URL, $url);

	// Desabilita a verificação da CA do servidor
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);

	// Set so curl_exec returns the result instead of outputting it.
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	//curl_setopt($ch, CURLOPT_CERTINFO, false);

	//seta o caminho do arquivo do chave privada
	//		curl_setopt($ch, CURLOPT_SSLKEY, $caminho_certificado);
	//		curl_setopt($ch, CURLOPT_SSH_PRIVATE_KEYFILE, $caminho_certificado);
	curl_setopt($ch, CURLOPT_SSLCERT, $caminho_certificado);
	//		curl_setopt($ch, CURLOPT_KEYPASSWD, $senha_certificado);
	//		curl_setopt($ch, CURLOPT_SSLKEYPASSWD, $senha_certificado);
	curl_setopt($ch, CURLOPT_SSLCERTPASSWD, $senha_certificado);

	// Get the response and close the channel.
	$response = curl_exec($ch);
	//ver(simec_htmlentities($response),d);
	curl_close($ch);

	return $response;

}

?>