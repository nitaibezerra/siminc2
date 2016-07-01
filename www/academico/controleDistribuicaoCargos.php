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
	switch ($_REQUEST['acao']) 
	{
		case 'salvarLancamento':
		      salvarLancamento($_REQUEST);
		    break;
		    
		case 'excluirLancamento':
		    excluirLancamento($_REQUEST);
		    break;
		    
		case 'carregaListaLancamentos':
		    carregaListaLancamentos($_REQUEST);
		    break;
		    
	    case 'salvarLancamentoDocentes':
	    	salvarLancamentoDocentes($_REQUEST);
	    break;
		    
		case 'carregarSaldoDisponivel':
		    carregarSaldoDisponivel($_REQUEST);
		    break;
		        
		case 'carregaSaldoLancado':
		    carregarSaldoLancado($_REQUEST);
		    break;
		case 'listarCampus':
		    listarCampus($_REQUEST);
		    break;
	}

	function excluirLancamento($params = Array())
	{
		global $db;
		
		$sql  = "UPDATE academico.lancamentocargos
                    SET                      
                        lnpstatus = 'I'
                    WHERE
                        lnpid	= ".$params['lnpid'];
		$db->executar($sql);
		$db->commit();

		
	}
	
	function salvarLancamento($params = Array())
	{
		global $db; 
        if($params['lnpid']=="null" || $params['lnpid']=="" )
        { 
            
           /* if(!validaDetalhes( $params ) )
            {
                 header("HTTP/1.0 404 OOOoooppzz");
                 echo "\nInserir Registro\nExiste lançamento para o cargo e campus  selecionado.\n\n\n";
                 return false;
            }*/
            
            /*$mensagem = validaLimite($params);
            
            //$mensagem ="";
            if(strlen($mensagem)>6)
            {
                 header("HTTP/1.0 404 OOOoooppzz");
                 echo "\n\n".$mensagem."\n\n";
                 return false;
            }*/
                       
            
            $sql = "INSERT INTO academico.lancamentocargos
                    (prtid, crgid, entidcampus, entidentidade, lnpvalor, lnpano, lnpdtinclusao)
                VALUES
                    (".$params['prtid'].",".$params['crgid'].",".$params['entidcampus'].",".$params['entidentidade'].",".$params['lancamento'].",'".$_SESSION['exercicio']."', now())
                RETURNING lnpid ";
           // die($sql);
            $lnpid = $db->pegaUm($sql);  
            $db->commit();
            

            /*$alerta = verificaDisponibilidadeAnos($params);
            if(strlen($alerta)>6)
            {
                 header("HTTP/1.0 2896886 Alerta");
                 echo "\n\n".$alerta."\n\n";
                 return true;
            }*/
            
		}
        else
        {
           /* $mensagem = validaLimiteUpdate($params);
            
            if(strlen($mensagem)>6)
            {
                 header("HTTP/1.0 404 OOOoooppzz");
                 echo "\n\n".$mensagem."\n\n";
                 return false;
            }*/
            
            $sql = "UPDATE academico.lancamentocargos
                    SET                      
                        lnpvalor = ". $params['lancamento'] ." 
                    WHERE
                        lnpid	= ".$params['lnpid'];
            
            $db->executar($sql);
            $db->commit();

            /*$alerta = verificaDisponibilidadeAnos($params);
            if(strlen($alerta)>6)
            {
                 header("HTTP/1.0 500 Alerta");
                 echo "\n\n".$alerta."\n\n";
                 return true;
            }*/
        }
	}
	
	function salvarLancamentoDocentes($params = Array())
	{
		global $db; 
        if($params['lnpid']=="null" || $params['lnpid']=="" )
        { 
            
           /* if(!validaDetalhes( $params ) )
            {
                 header("HTTP/1.0 404 OOOoooppzz");
                 echo "\nInserir Registro\nExiste lançamento para o cargo e campus  selecionado.\n\n\n";
                 return false;
            }*/
            
            /*$mensagem = validaLimite($params);
            
            //$mensagem ="";
            if(strlen($mensagem)>6)
            {
                 header("HTTP/1.0 404 OOOoooppzz");
                 echo "\n\n".$mensagem."\n\n";
                 return false;
            }*/
                       
            
            $sql = "INSERT INTO academico.lancamentocargos
                    (prtid, crgid, entidcampus, entidentidade, lnpvalor, lnpano, lnpdtinclusao)
                VALUES
                    (".$params['prtid'].",".$params['crgid'].",".$params['entidcampus'].",".$params['entidentidade'].",".$params['lancamento'].",'".$_SESSION['exercicio']."', now())
                RETURNING lnpid ";
           // die($sql);
            $lnpid = $db->pegaUm($sql);  
            $db->commit();
            

            /*$alerta = verificaDisponibilidadeAnos($params);
            if(strlen($alerta)>6)
            {
                 header("HTTP/1.0 2896886 Alerta");
                 echo "\n\n".$alerta."\n\n";
                 return true;
            }*/
            
		}
        else
        {
           /* $mensagem = validaLimiteUpdate($params);
            
            if(strlen($mensagem)>6)
            {
                 header("HTTP/1.0 404 OOOoooppzz");
                 echo "\n\n".$mensagem."\n\n";
                 return false;
            }*/
            
            $sql = "UPDATE academico.lancamentocargos
                    SET                      
                        lnpvalor = ". $params['lancamento'] ." 
                    WHERE
                        lnpid	= ".$params['lnpid'];
            
            $db->executar($sql);
            $db->commit();

            /*$alerta = verificaDisponibilidadeAnos($params);
            if(strlen($alerta)>6)
            {
                 header("HTTP/1.0 500 Alerta");
                 echo "\n\n".$alerta."\n\n";
                 return true;
            }*/
        }
	}
	
	function validaLimite($params = Array())
	{

        $params['callback'] = true;
		$disponivel = carregarSaldoDisponivel($params);
        /*
        echo "<pre>";
        print_r($disponivel);
        */   
		$mensagem = "";

		if($params['lnpvalor'] > $disponivel['lnpvalor'])
			$mensagem .= "Lançamento está acima do limite disponível: ".  $disponivel['lnpvalor']."\n";
		
		if(strlen($mensagem)>5)
			return $mensagem;
		else
			return true;
	}

	
	
	function verificaDisponibilidadeAnos($params = Array())
	{
		$params['callback'] = true;
        $disponivel = carregarSaldoDisponivel($params);
        $mensagem = "";
		if($params['lnpvalor'] == 0 && $disponivel['lnpvalor'] > 0)
			$mensagem .= "Existem ".$disponivel['lnpvalor']." vagas que você pode utilizar. \n";		
        
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
		$disponivel = carregarSaldoDisponivel($params);
        
        $sql = "SELECT 
                    lnpvalor
                FROM  
                    academico.lancamentocargos
                WHERE
                    prtid = ". $params['prtid'];

        $registro = $db->carregar($sql);
        if(is_array($registro))
            $registro = array_pop($registro);
        //print_r(  $registro);
        
		$mensagem = "";
		
		if($params['lnpvalor'] > ($disponivel['lnpvalor'] + $registro['lnpvalor']))
			$mensagem .= "Lançamento está acima do limite disponível: ".  $disponivel['lnpvalor']."\n";
				
		if(strlen($mensagem)>5)
			return $mensagem;
		else
			return true;
	}    
    
	function validaDetalhes($params = Array())
	{
		global $db;

        $sql = "SELECT prtid FROM reuni.campuslancamento
                WHERE
		    limctpreg  = 'C' AND
                    entidcampus  = ".$params['entidcampus']."
                AND	clsid  = ".$params['clsid'];
                if($params['carcod']=='null' ||$params['carcod']==null)
                    $sql .=	" AND	carcod is null";
                else
                    $sql .=	" AND	carcod = ".$params['carcod'];

		$prtid = $db->carregar($sql);

		if(!is_array($prtid))
			return true;	

		$prtid = array_pop($prtid);

		if(count($prtid)>=1)
		{
			return false;
		}else{
			return true;
		}
	}

	
	function carregaListaLancamentos($params = Array())
	{
		global $db;
        pg_set_client_encoding('UTF-8');
        $sql = "SELECT
					un.entnome as universidade,
					un.entid as entidentidade,
					cu.entnome as campus,
					cu.entid as entidcampus,
					lc.lnpvalor as lancamento, 					
					CASE WHEN pr.prjnumero is null THEN 0  ELSE pr.prjnumero END as projetado,
					ca.crgdsc as cargo,
					lc.prtid,
					lc.lnpid, 
					lc.crgid					
				
				FROM 
					academico.lancamentocargos AS lc
					
				INNER JOIN entidade.entidade AS un ON (un.entid  = lc.entidentidade)
				INNER JOIN entidade.entidade AS cu ON (cu.entid  = lc.entidcampus)
				INNER JOIN academico.cargos AS ca ON (ca.crgid  = lc.crgid)
				INNER JOIN academico.classes AS cls ON (cls.clsid  = ca.clsid)
				LEFT JOIN academico.projetados AS pr ON (pr.crgid  = ca.crgid)
				
				WHERE
					lc.prtid  			= ".$params['prtid']." 			AND
					lc.entidentidade 	= ".$params['entidentidade']." 	AND
					lc.entidcampus 		= ".$params['entidcampus']." 	AND
					cls.clsid  			= ".$params['clsid']." 			AND
					lc.lnpstatus 		= 'A' 							AND
					lc.lnpano 			= '".$_SESSION['exercicio']."'
				ORDER BY
				un.entnome, cu.entnome, ca.crgdsc";
		//die($sql);    
		$valores = $db->carregar($sql);
        pg_set_client_encoding('LATIN5');
        $var     = simec_json_encode($valores);
        
        echo $var;
        
	}

	function carregarSaldoDisponivel($params = Array())
	{	
        
		global $db;
		
		 $sql = "SELECT
                    lnpvalor
                FROM 
                    reuni.limitecargo
                WHERE
		    limtpreg    = 'C' AND
                    unicod      = '".$params['unicod']."' AND
                    unitpocod   = '".$params['unitpocod']. "' AND
                    clsid       = ".$params['clsid'];

		//pega o limite e....
		$limite = $db->carregar($sql);
		
        if(is_array($limite)) {
            $limite = array_pop($limite);
        } else {
        	echo false;
			exit;
        }
      
	
	//!!!
        if($params['callback']==true)
            $callback = true;
        else
            $callback = false;
        
        $params['callback'] = true;
        //subtrai o que foi lançado...
		$lancados = carregarSaldoLancado($params);
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

	function carregarSaldoLancado($params = Array())
	{

		global $db;
		
		$sql = "SELECT
				sum(lc.lnpvalor) as lnpvalor
				cl.clsid
		
			FROM 
				academico.lancamentocargos 	AS lc
			
				INNER JOIN reuni.campusuniversitario AS cu ON (cu.entidcampus  = lc.cauid)
				INNER JOIN reuni.campuslancamento    AS cl ON (cl.prtid  = cast(lc.prtid as integer))
				LEFT  JOIN reuni.cargo		     AS ca ON (ca.carcod = cl.carcod)
			
			WHERE
				cl.limctpreg  = 'C' AND
				cu.unicod     = '".$params['unicod']."' AND
				cu.unitpocod  = '".$params['unitpocod'] . "' AND                
				cl.clsid      =  ".$params['clsid']	. "
			GROUP BY cl.clsid";

		$valores = $db->carregar($sql);

		if(!is_array($valores))		
		{
			$valores = Array();
			$valores[0]['clsid']    = $params['clsid'];
			$valores[0]['lnpvalor']  = '0';
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
	
	function listarCampus($params = Array())
	{
		global $db;
		
		$entidentidade = $params['entidentidade'];
		$sql = "select entid as codigo, entnome as descricao from entidade.entidade where funid = 18 and entidassociado = ".$entidentidade."";
		$dados = $db->carregar($sql);
		
		$enviar = '';
		if($dados) $dados = $dados; else $dados = array();
		$enviar .= "<option value=0>Selecione...</option> \n";
		foreach ($dados as $data) {
			$enviar .= "<option value= ".$data['codigo'].">  ".simec_htmlentities($data['descricao'])." </option> \n";
		}				
		die($enviar);
	}	
	
	
	function listarCampus2($params = Array())
	{
		global $db;
		$enviar = '';
		$entidentidade = $params['entidentidade'];
		$entidcampus = $params['entidcampus'];
		if($entidentidade){
			
			$sql = "select entid as codigo, entnome as descricao from entidade.entidade where funid = 18 and entidassociado = ".$entidentidade."";			
		
		/*	$dados = $db->carregar($sql);
			$action = "onchange='carregarAbas(".$entidentidade.", this.value)'";
			$enviar .= "<option value=\"\"> Selecione... </option> \n";
			foreach ($dados as $data) {
				$enviar .= "<option value= ".$data['codigo'].">  ".simec_htmlentities($data['descricao'])." </option> \n";
			}*/
			
		}else{
			//$enviar .= "<option value=\"\"> Selecione... </option> \n";
			$sql ="";
		}	
				
		//die($enviar);
		
		die($db->monta_combo('entidcampus',$sql,'S','Selecione...','carregarAbas','','','100%','N', 'entidcampus'));
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