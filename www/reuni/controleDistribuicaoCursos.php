<?php

	/*
	Sistema Simec
	Setor responsável: SPO-MEC
	Analista: Cristiano Cabral (cristiano.cabral@gmail.com), Bruno Adann Sagretzki Coura, Henrique Xavier Couto
	Programador: Mário César Gasparini Nascimento (pilpas@gmail.com)
	Módulo: www/geral/controleDistribuicaoCargos.php
	Finalidade: Centralizar as ações de Manutenção da tela de Plano de Distribuição de Cargos do Módulo:
		[ /reuni/modulos/principal/planodistribuicaocargos.inc
	*/

	include "config.inc";
	header( 'Cache-Control: no-store, no-cache, must-revalidate' );
	header( 'Cache-Control: post-check=0, pre-check=0', false );
	header( 'Cache-control: private, no-cache' );
	header( 'Last-Modified: ' . gmdate( 'D, d M Y H:i:s' ) . ' GMT' );
	header( 'Pragma: no-cache' );
	header( 'Content-Type: text/html; charset=iso-8859-1' );

	include APPRAIZ."includes/classes_simec.inc";
	include APPRAIZ."includes/funcoes.inc";
	$db = new cls_banco();

	//caso o lançamento seja
	foreach ( $_REQUEST as $chave => $valor )	
	{
		if(substr($chave, 0, 3) =="lan")
			if ($valor==0|| $valor=='')
				$_REQUEST[$chave] = 0;
	}

	// pra pegar o unitpocod.
	if($_REQUEST['unicod'])
	{
		$sql = "SELECT unitpocod FROM unidade WHERE UNICOD = '".$_REQUEST['unicod']."'";
		$rs = $db->pegaUm($sql);
		$_REQUEST['unitpocod'] = $rs;
	}

    
    	//TODAS AS AÇÕES SÃO PARA CARGOS.
	switch ($_REQUEST['metodo']) 
	{
		case 'carregarSaldoLancadoCurso':
		    carregarSaldoLancadoCurso($_REQUEST);
		    break;

		case 'carregarSaldoDisponivelCurso':
		    carregarSaldoDisponivelCurso($_REQUEST);
		    break;

	
		case 'salvarLancamentoCurso':
		      salvarLancamentoCurso($_REQUEST);
		    break;
		   
	    	case 'carregaListaLancamentos':
		    carregaListaLancamentos($_REQUEST);
		    break; 		    
		    
	    	case 'excluirLancamento':
		    excluirLancamento($_REQUEST);
		    break;
	}

	function excluirLancamento($params = Array())
	{
		global $db;

		$sql  = "DELETE from  reuni.cursolancamento WHERE culid = ".$params['culid'];
		$db->executar($sql);
		$db->commit();
	}
	
	function salvarLancamentoCurso($params = Array())
	{
		global $db;
		/*
		echo "<pre>";
		print_r($params);
		*/
		
		if($params['culid']=="null" || $params['culid']=="" )
		{
		    
		    if(!validaDetalhes( $params ) )
		    {
			 header("HTTP/1.0 404 OOOoooppzz");
			 echo "\nInserir Registro\nExiste lançamento para o Curso selecionado.\n\n\n";
			 return false;
		    }
		    
		    $mensagem = validaLimite($params);
		    
		    //$mensagem ="";
		    if(strlen($mensagem)>6)
		    {
			 header("HTTP/1.0 404 OOOoooppzz");
			 echo "\n\n".$mensagem."\n\n";
			 return false;
		    }
		    
		    $sql = "INSERT INTO reuni.cursolancamento
			    	(
					nlcid,
					curid,
					lan2008,
					lan2009,
					lan2010,
					lan2011,
					lan2012 
				)
			    VALUES
			    	(
					".$params['nlcid'].",
					".$params['curid'].",
					".$params['lan2008'].",
					".$params['lan2009'].",
					".$params['lan2010'].",
					".$params['lan2011'].",					
					".$params['lan2012']."
				)";
		    //die();
		    $db->executar($sql);
		    $db->commit();
		    //$db->rollback();

		    $alerta = verificaDisponibilidadeAnos($params);
		    if(strlen($alerta)>6)
		    {
			 header("HTTP/1.0 2896886 Alerta");
			 echo "\n\n".$alerta."\n\n";
			 return true;
		    }
		    
		}
		else
		{
		    $mensagem = validaLimiteUpdate($params);
		    
		    if(strlen($mensagem)>6)
		    {
			 header("HTTP/1.0 404 OOOoooppzz");
			 echo "\n\n".$mensagem."\n\n";
			 return false;
		    }
		    
		    $sql = "UPDATE reuni.cursolancamento
			    SET
				curid   = ". $params['curid']   ." ,
				lan2008 = ". $params['lan2008'] ." ,
				lan2009 = ". $params['lan2009'] ." ,
				lan2010 = ". $params['lan2010'] ." ,
				lan2011 = ". $params['lan2011'] ." ,
				lan2012 = ". $params['lan2012'] ." 
			    WHERE
				culid	= ".$params['culid'];
		    
		    $db->executar($sql);
		    $db->commit();
	
		    $alerta = verificaDisponibilidadeAnos($params);
		    if(strlen($alerta)>6)
		    {
			 header("HTTP/1.0 500 Alerta");
			 echo "\n\n".$alerta."\n\n";
			 return true;
		    }
		}
	}
	
	function validaLimite($params = Array())
	{

		$params['callback'] = true;
		$disponivel = carregarSaldoDisponivelCurso($params);
         /*
        echo "<pre>";
        print_r($disponivel);
       */   
		$mensagem = "";

		if($params['lan2008'] > $disponivel['lan2008'])
			$mensagem .= "Lançamento do ano 2008 está acima do limite disponível: ".  $disponivel['lan2008']."\n";
		
		if($params['lan2009'] > $disponivel['lan2009'])
			$mensagem .= "Lançamento do ano 2009 está acima do limite disponível: ". $disponivel['lan2009']."\n";
		
		if($params['lan2010'] > $disponivel['lan2010'])
			$mensagem .= "Lançamento do ano 2010 está acima do limite disponível: ". $disponivel['lan2010']."\n";
		
		if($params['lan2011'] > $disponivel['lan2011'])
			$mensagem .= "Lançamento do ano 2011 está acima do limite disponível: ". $disponivel['lan2011']."\n";
		
		if($params['lan2012'] > $disponivel['lan2012'])
			$mensagem .= "Lançamento do ano 2012 está acima do limite disponível: ". $disponivel['lan2012']."\n";
		
		if(strlen($mensagem)>5)
			return $mensagem;
		else
			return true;
	}

	
	
	function verificaDisponibilidadeAnos($params = Array())
	{
		$params['callback'] = true;
		$disponivel = carregarSaldoDisponivelCurso($params);
		$mensagem = "";
		if($params['lan2008'] == 0 && $disponivel['lan2008'] > 0)
			$mensagem .= "Existem ".$disponivel['lan2008']." vagas para 2008 que você pode utilizar. \n";
		
		if($params['lan2009'] == 0 && $disponivel['lan2009'] > 0)
			$mensagem .= "Existem ".$disponivel['lan2009']." vagas para 2009 que você pode utilizar. \n";
		
		if($params['lan2010'] == 0 && $disponivel['lan2010'] > 0)
			$mensagem .= "Existem ".$disponivel['lan2010']." vagas para 2010 que você pode utilizar. \n";
		
		if($params['lan2011'] == 0 && $disponivel['lan2011'] > 0)
			$mensagem .= "Existem ".$disponivel['lan2011']." vagas para 2011 que você pode utilizar. \n";
		
		if($params['lan2012'] == 0 && $disponivel['lan2012'] > 0)
			$mensagem .= "Existem ".$disponivel['lan2012']." vagas para 2012 que você pode utilizar. \n";
        
        return $mensagem;
    }
    
	function validaLimiteUpdate($params = Array())
	{
		global $db;
		/* 
		echo"<pre>";
		print_r($params);
		//$params = array_push()
		 */
		$params['callback'] = true;
			$disponivel = carregarSaldoDisponivelCurso($params);
		
		$sql = "SELECT 
			    lan2008,lan2009,lan2010,lan2011,lan2012 
			FROM  
			    reuni.cursolancamento
			WHERE
			    culid = ". $params['culid'];

		$registro = $db->carregar($sql);
		if(is_array($registro))
		    $registro = array_pop($registro);
		//print_r(  $registro);
		
		$mensagem = "";
		
		if($params['lan2008'] > ($disponivel['lan2008'] + $registro['lan2008']))
			$mensagem .= "Lançamento do ano 2008 está acima do limite disponível: ".  $disponivel['lan2008']."\n";
		
		if($params['lan2009'] > ($disponivel['lan2009']+ $registro['lan2009']))
			$mensagem .= "Lançamento do ano 2009 está acima do limite disponível: ". $disponivel['lan2009']."\n";
		
		if($params['lan2010'] > ($disponivel['lan2010']+ $registro['lan2010']))
			$mensagem .= "Lançamento do ano 2010 está acima do limite disponível: ". $disponivel['lan2010']."\n";
		
		if($params['lan2011'] > ($disponivel['lan2011']+ $registro['lan2011']))
			$mensagem .= "Lançamento do ano 2011 está acima do limite disponível: ". $disponivel['lan2011']."\n";
		
		if($params['lan2012'] > ($disponivel['lan2012']+ $registro['lan2012']))
			$mensagem .= "Lançamento do ano 2012 está acima do limite disponível: ". $disponivel['lan2012']."\n";
		
		if(strlen($mensagem)>5)
			return $mensagem;
		else
			return true;
	}    
    
	function validaDetalhes($params = Array())
	{
		global $db;

		$sql = "SELECT
			   cl.culid 
		        FROM
			   reuni.cursolancamento as cl

			   INNER JOIN reuni.solicitacaocurso as sc on (sc.curid = cl.curid)
		        WHERE
			    cl.nlcid  = ".$params['nlcid']." AND
			    cl.curid  = ".$params['curid']." AND
			    sc.unicod = '".$params['unicod']."'";
		/*
		if($params['carcod']=='null' ||$params['carcod']==null)
		    $sql .=	" AND	carcod is null";
		else
		    $sql .=	" AND	carcod = ".$params['carcod'];
		*/

		$culid = $db->carregar($sql);

		if(!is_array($culid))
			return true;	

		$culid = array_pop($culid);

		if(count($culid)>=1)
		{
			return false;
		}else{
			return true;
		}
	}

	
	function carregaListaLancamentos($params = Array())
	{
		//echo "xxxx";
		global $db;
		pg_set_client_encoding('UTF-8');
		$sql = "
		    SELECT
					cl.culid,
					cl.curid,
					sl.curnome,
					cl.lan2008,
					cl.lan2009,
					cl.lan2010,
					cl.lan2011,
					cl.lan2012
				FROM 
					reuni.cursolancamento 	AS cl
					INNER JOIN reuni.solicitacaocurso AS sl ON (sl.curid  = cl.curid)
				WHERE
					sl.unicod     = '".$params['unicod']."' AND
					cl.nlcid      = ".$params['nlcid']."
				ORDER BY
					sl.curnome";

		$valores = $db->carregar($sql);
		pg_set_client_encoding('LATIN5');
		$var     = simec_json_encode($valores);
		
		echo $var;
	}

	function carregarSaldoDisponivelCurso($params = Array())
	{

		global $db;

		$sql = "SELECT
			    lan2008,
			    lan2009,
			    lan2010,
			    lan2011,
			    lan2012
			FROM
			    reuni.limitecargo
			WHERE
			    nlcid       =  ".$params['nlcid'].     "  AND
			    unicod      =  '".$params['unicod']."'  AND
			    unitpocod   = '".$params['unitpocod']. "' AND
			    limtpreg    = '".$params['limtpreg']. "' ";

			
		//pega o limite e....		
		$limite = $db->carregar($sql);
		if(is_array($limite))
		    $limite = array_pop($limite);
	    	else
		{
			$limite['lan2008']  	= '0';
			$limite['lan2009']  	= '0';
			$limite['lan2010']  	= '0';
			$limite['lan2011']  	= '0';
			$limite['lan2012']  	= '0';
		}

		//!!!
		if($params['callback']==true)
		    $callback = true;
		else
		    $callback = false;
		
		$params['callback'] = true;
		//subtrai o que foi lançado...
		$lancados = carregarSaldoLancadoCurso($params);
		$params['callback'] = false;        
	

		if(is_array($lancados))
			$lancados = array_pop($lancados);
		else
			$disponivel = $limite;

		$disponivel = array();
		
		foreach ( $limite as $chave => $valor ) 
			$disponivel[$chave] = $limite[$chave] - $lancados[$chave];

		if($callback)
			return $disponivel;
		else
		{
			$geraXML = new CArray2xml2array();
			$geraXML->setArray($disponivel);
			
			if($geraXML->saveArray("valores.xml"))
			{
				$handle = fopen ("valores.xml", "r");
				header('content-type: text/xml; charset=ISO-8859-1');
				while (!feof($handle)) {
					$buffer = fgets($handle, 4096);
					echo $buffer;
				}
				unlink(APPRAIZ ."www/reuni/valores.xml");
			}
		}
	}

	function carregarSaldoLancadoCurso($params = Array())
	{
		global $db;
		
		$sql = "SELECT
				sum(cl.lan2008) AS lan2008,
				sum(cl.lan2009) AS lan2009,
				sum(cl.lan2010) AS lan2010,
				sum(cl.lan2011) AS lan2011, 
				sum(cl.lan2012) AS lan2012,  
				cl.nlcid
			FROM
				reuni.cursolancamento AS cl
				INNER JOIN reuni.solicitacaocurso     AS  sc  ON (sc.curid = cl.curid)
			WHERE 
				cl.nlcid     =  ". $params['nlcid'].    "  AND
				sc.unicod    =  '". $params['unicod']."'  AND
				sc.limtpreg  = '". $params['limtpreg']. "'
			GROUP BY
				cl.nlcid";
		$valores = $db->carregar($sql);

		if(!is_array($valores))		
		{
			$valores = Array();
			$valores[0]['limtpreg']    	= $params['limtpreg'];
			$valores[0]['lan2008']  	= '0';
			$valores[0]['lan2009']  	= '0';
			$valores[0]['lan2010']  	= '0';
			$valores[0]['lan2011']  	= '0';
			$valores[0]['lan2012']  	= '0';
		}
		
		if($params['callback'])
			return $valores;
		else
		{
			$geraXML = new CArray2xml2array();
			$geraXML->setArray($valores);

			if($geraXML->saveArray("valores.xml"))
			{
				$handle = fopen ("valores.xml", "r");
				header('content-type: text/xml; charset=ISO-8859-1');
				while (!feof($handle)) {
					$buffer = fgets($handle, 4096);
					echo $buffer;
				}
				unlink(APPRAIZ ."www/reuni/valores.xml");
			}				
			
		}

	}


/**
* associative array to xml transformation class
*  
* @PHPVER    5.0
*
* @author    Johnny Brochard
* @ver        0001.0002
* @date    25/08/04
*/


class CArray2xml2array {

    /*
     * XML Array
     * @var array
     * @access private
     */
    private $XMLArray;

    /*
     * array is OK
     * @var bool
     * @access private
     */
    private $arrayOK;

    /*
     * XML file name
     * @var string
     * @access private
     */
    private $XMLFile;

    /*
     * file is present
     * @var bool
     * @access private
     */
    private $fileOK;

    /*
     * DOM document instance
     * @var DomDocument
     * @access private
     */
    private $doc;

    /**
     * Constructor
     * @access public
     */

    public function __construct(){
        
    }

    /**
     * setteur setXMLFile
     * @access public
     * @param string $XMLFile
     * @return bool
     */

    public function setXMLFile($XMLFile){
        if (file_exists($XMLFile)){
            $this->XMLFile = $XMLFile;
            $this->fileOK = true;
        }else{
            $this->fileOK = false;
        }
        return $this->fileOK;
    }

    /**
     * saveArray
     * @access public
     * @param string $XMLFile
     * @return bool
     */

    public function saveArray($XMLFile, $rootName="", $encoding="UTF-8"){
        global $debug;
        $this->doc = new domdocument("1.0", $encoding);
        $arr = array();
        if (count($this->XMLArray) > 1){
            if ($rootName != ""){
                $root = $this->doc->createElement($rootName);
            }else{
                $root = $this->doc->createElement("resultado");
                $rootName = "resultado";
            }
            $arr = $this->XMLArray;
        }else{

            $key = key($this->XMLArray);
            $val = $this->XMLArray[$key];

            if (!is_int($key)){
                $root = $this->doc->createElement($key);
                $rootName = $key;
            }else{
                if ($rootName != ""){
                    $root = $this->doc->createElement($rootName);
                }else{
                    $root = $this->doc->createElement("resultado");
                    $rootName = "resultado";
                }
            }
            $arr = $this->XMLArray[$key];
        }
        
        $root = $this->doc->appendchild($root);
    
        $this->addArray($arr, $root, $rootName);

/*        foreach ($arr as $key => $val){
            $n = $this->doc->createElement($key);
            $nText = $this->doc->createTextNode($val);
            $n->appendChild($nodeText);
            $root->appendChild($n);
        }
*/        
        
        if ($this->doc->save($XMLFile) == 0){
            return false;
        }else{
            return true;
        }
    }

    /**
     * addArray recursive function
     * @access public
     * @param array $arr
     * @param DomNode &$n
     * @param string $name
     */

    function addArray($arr, &$n, $name=""){
        foreach ($arr as $key => $val){
            if (is_int($key)){
                if (strlen($name)>1){
                    $newKey = substr($name, 0, strlen($name)-1);
                }else{
                    $newKey="item";
                }
            }else{
                $newKey = $key;
            }

            $node = $this->doc->createElement($newKey);
            if (is_array($val)){
                $this->addArray($arr[$key], $node, $key);
            }else{
                $nodeText = $this->doc->createTextNode($val);
                $node->appendChild($nodeText);
            }
            $n->appendChild($node);
        }
    }

    
    /**
     * setteur setArray
     * @access public
     * @param array $XMLArray
     * @return bool
     */

    public function setArray($XMLArray){
        if (is_array($XMLArray) && count($XMLArray) != 0){
            $this->XMLArray = $XMLArray;
            $this->arrayOK = true;
        }else{
            $this->arrayOK = false;
        }
        return $this->arrayOK;
    }

}
?>