 <?
  /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
	Thiago Henrique Ramos da Mata( e-mail: thiago.henrique.mata@gmail.com )
   módulo:importacsv.inc
   
   finalidade: permitir importar arquivo csv para dentro do plano gerencial
   */
  function codigopai($id)
  {
  	global $db;
  	$sql = "select ptoid_pai,ptoid, ptocod,ptoordemprov from monitora.planotrabalho where ptoid=$id";
  	$cod = $db->pegalinha($sql);

  	if ($cod['ptoid_pai'])
  	{
  		// entao tenho pai 
  		return codigopai($cod['ptoid_pai'] ).$cod['ptoordemprov'];

  	}
  	else 
  	{
  		  	
  	return $cod['ptoordemprov'];
  	}
  }
   $modulo=$_REQUEST['modulo'] ;//
/*   $i=0;
   $sql = "update monitora.planotrabalho set ptoordemprov=null";
   $db->executar($sql);
   $sql = "select pjeid, ptoid, ptonivel from monitora.planotrabalho where ptostatus='A' and ptoordemprov is null and pjeid is not null order by ptonivel";
   $listas = $db->carregar($sql);
      	  $j=0;
   foreach ($listas as $lista)
   {
   	  $i = 0;
   	  $busca = $lista['ptoid'];
   	  $pjeid = $lista['pjeid'];
   	  $sql = "select ptoid  from monitora.planotrabalho where pjeid=$pjeid and ptostatus='A' and ptoordemprov is null and ptoid_pai = $busca";
   	  $nlistas = $db->carregar($sql);
   	  if (is_array($nlistas))
   	  {
   	  foreach ($nlistas as $nlista)
   	  {
   	  	$i++;
   	  	$id=$nlista['ptoid'];
   	  	$sql = "update monitora.planotrabalho set ptoordemprov=$i where pjeid=$pjeid and ptoid=$id";
   	  	$db->executar($sql);
   	  }
   	  }
   }
   $sql = "select pjeid, ptoid, ptonivel from monitora.planotrabalho where ptostatus='A' and ptoordemprov is null and pjeid is not null and ptoid_pai is null order by ptonivel";
   $listas = $db->carregar($sql);

 /*  foreach ($listas as $lista)
   {   	  
   	  $busca = $lista['ptoid'];
   	  $pjeid = $lista['pjeid'];
   	     $i = 0;
   	  $sql = "select pjeid, ptoid, ptonivel from monitora.planotrabalho where ptostatus='A' and ptoordemprov is null and pjeid =$pjeid and ptoid_pai is null order by ptonivel";
   	  $nlistas = $db->carregar($sql);
   	  if (is_array($nlistas))
   	  {
   	  foreach ($nlistas as $nlista)
   	  {
   	  	$i++;
   	  	$id=$nlista['ptoid'];
   	  	$sql = "update monitora.planotrabalho set ptoordemprov=$i where pjeid=$pjeid and ptoid=$id";
   	  	$db->executar($sql);
   	  }
   	  }
   }
   */
 // crio os códigos
   $sql = "update monitora.planotrabalho set ptocod=null where ptonivel > 1";
   $db->executar($sql);
 $sql = "select pjeid, ptoid, ptonivel,ptoordemprov from monitora.planotrabalho where ptostatus='A' and ptoordemprov is not null and pjeid is not null and ptoid_pai is not null order by ptonivel";
 $listas = $db->carregar($sql);

 foreach ($listas as $lista)
 {
 	
 	$meucodigo = codigopai($lista['ptoid']);
 	//$meucodigo = $meucodigo.$lista['ptoordemprov'];
 	 dbg($meucodigo);
 	$busca = (integer) $lista['ptoid'];
   	$pjeid = (integer) $lista['pjeid'];
 	$sql = "update monitora.planotrabalho set ptocod='$meucodigo' where pjeid=$pjeid and ptostatus='A' and ptoid = $busca ";
 	//if ($busca == 1288) dbg($sql,1);
 	//dbg($sql);
 	$db->executar($sql);
 }
 
   $db->commit();
   dbg(1,1);
   
class ElementoNivel
{
	/**
	 * @var object database connection
	 */		
	static protected $objDatabase;

	
	protected $intId			= null;
	protected $strNome		 	= '';
	protected $intNivel			= 0;
	protected $strDescricao		= '';
	protected $objPai			= null;
	protected $intLinha			= null;
	protected $strDataInicio 	= null;
	protected $strDataFim	 	= null;
	protected $strUsuarioCPF 	= null;
	protected $intLinhaAntecessor	= null;
	
	public function setNome( $strNome )
	{
		if( $strNome !== null )
		{
			$this->strNome = $strNome;
		}
		else
		{
			$this->strNome = null;
		}
	}
	
	public function getNome()
	{
		return $this->strNome;
	}
		
	public function setDataInicio( $strDataInicio )
	{
		$this->strDataInicio = $strDataInicio;
	}
		
	public function getDataInicio()
	{
		return $this->strDataInicio;
	}
	
	public function setDataFim( $strDataFim )
	{
		$this->strDataFim = $strDataFim;
	}
		
	public function getDataFim()
	{
		return $this->strDataFim;
	}
	
	public function setUsuarioCPF( $strUsuarioCPF )
	{
		if( strlen( $strUsuarioCPF ) < 11 and $strUsuarioCPF <>'')
		{
			$strUsuarioCPF = str_repeat( '0' , 11 - strlen( $strUsuarioCPF ) ) . $strUsuarioCPF;
		}
		$this->strUsuarioCPF  = $strUsuarioCPF; 
	}
	
	public function getUsuarioCPF()
	{
		return $this->strUsuarioCPF;
	}
	
	public function setId( $intId )
	{
		$this->intId = $intId;
	}
	
	public function getId()
	{
		return $this->intId;
	}
	
	public function setLinhaAntecessor( $intLinhaAntecessor )
	{
		$this->intLinhaAntecessor = $intLinhaAntecessor;
	}
	
	public function getLinhaAntecessor()
	{
		return $this->intLinhaAntecessor;
	}
		
	public function setLinha( $intLinha )
	{
		$this->intLinha = $intLinha;
	}
	
	public function getLinha()
	{
		return $this->intLinha;
	}
		
	public function setNivel( $intNivel )
	{
		$this->intNivel = $intNivel;
	}
	
	public function getNivel()
	{
		return $this->intNivel;
	}
	
	public function setDescricao( $strDescricao )
	{
		$this->strDescricao = $strDescricao;
	}
	
	public function getDescricao()
	{
		return $this->strDescricao;
	}
	
	public function setPai( ElementoNivel $objPai )
	{
		$this->objPai = $objPai;
	}
	
	public function getPai()
	{
		if( $this->objPai == null )
		{
			$this->objPai = new ElementoNivel();
		}
		return $this->objPai;
	}
	
	public function __construct()
	{
		global $db;
		self::$objDatabase = $db;
	}
	
	public function validar()
	{
		
	}
	
	/**
	 * Insere a tarefa na persistencia após validar os seus campos
	 * 
	 * 1. Valida a tarefa
	 * 2. Insere a tarefa na persistencia
	 * 3. Atualiza o identificador da tarefa
	 */
	public function insert()
	{
		global $db;
		# 1. Valida a tarefa										#
		$this->validar();
		
		# 2. Insere a tarefa na persistencia						#
			
			// gerando a query //
			
		$strSql = " 
		INSERT INTO	monitora.temp
			( 
				ptonivel, 
				ptodsc,
				ptodescricao, 
				ptoid_pai,
				id,
				ptodata_ini,
				ptodata_fim,
				ptoid_antecessor,
				usucpf 
			)
		VALUES
			(
				" . self::$objDatabase->escape( $this->intNivel ) . ",
				" . self::$objDatabase->escape( $this->strNome ) . ",
				" . self::$objDatabase->escape( $this->strDescricao ) . ",
				" . self::$objDatabase->escape( $this->getPai()->getLinha() ) . ",
				" . self::$objDatabase->escape( $this->intLinha ) . ",
				" . self::$objDatabase->escape( $this->strDataInicio ) . ",
				" . self::$objDatabase->escape( $this->strDataFim ) . ",
				" . self::$objDatabase->escape( $this->intLinhaAntecessor ).",
				" . self::$objDatabase->escape( $this->strUsuarioCPF ) . "
			)	
		";

		// executando a query criada //
		//dbg( $strSql );
		$objSaida = self::$objDatabase->executar( $strSql );
		# 3. Atualiza o identificador da tarefa						#
		//$this->setId( (integer) $objSaida );
				

		return;
	}
	
} 
//error_reporting( E_ALL );
function ImportaCSV( $filename )
{
	$arrUltimoNoNivel		= array();
	$objRaiz				= new ElementoNivel();
	$arrUltimoNoNivel[ 0 ]	= $objRaiz;
	$arrUltimoNoNivel[ 1 ]	= $objRaiz;
	$arrUltimoNoNivel[ 2 ]	= $objRaiz;
	$arrUltimoNoNivel[ 3 ]	= $objRaiz;
	$arrObjElementos		= array();
	
	//ini_set("memory_limit","512M");
     //set_time_limit(0);
	$arrCvsFile = file( APPRAIZ . "www/" . $filename );	
  	$strCabecalho = array_shift( $arrCvsFile );
  	$arrLinha = explode( ';' , $strCabecalho );
    foreach ( $arrCvsFile as $intLinha => $strLinhaAtual )
     {
     	$arrLinha = explode( ';' , $strLinhaAtual );
		$objElemento			= new ElementoNivel();
		$objElemento->setLinha(			(integer)	$arrLinha[ 0 ] );
		$objElemento->setNome( 			(string)	trim( $arrLinha[ 1 ] ) );
		$objElemento->setDataInicio(	(string)	trim( $arrLinha[ 2 ] ) );
		$objElemento->setDataFim(		(string)	trim( $arrLinha[ 3 ] ) );
		$objElemento->setLinhaAntecessor( 	(integer)	$arrLinha[ 4 ] );
		$objElemento->setUsuarioCPF(	(string)	trim($arrLinha[ 5 ] ) );
		$objElemento->setDescricao( 	(string)	trim($arrLinha[ 6 ] ) );
		$objElemento->setNivel(			(integer)	$arrLinha[ 7 ] );
		$objElemento->setPai( $arrUltimoNoNivel[  (integer)$objElemento->getNivel() - 1 ] );
		$arrUltimoNoNivel[ $objElemento->getNivel() ] = $objElemento;
		$arrObjElementos[] = $objElemento;
		
     }


     foreach( $arrObjElementos as $objElemento )
     {
     	$objElemento->insert();
     };

    // $resultado = pg_query('begin transaction; ');
}

   if ($_REQUEST['act']=='1')
   {
   	$db->executar('delete from monitora.temp');
	importaCSV($_REQUEST['arquivo']);
	$db->commit();
}



   if ($_REQUEST['act']=='2')
   {
    $sql = "select ptoid as id, ptoordemprov from monitora.planotrabalho where ptoordemprov is not null";
    $lista = $db->carregar($sql);
    foreach ($lista as $linha)
    {
    	$sql = "select ptoordem from monitora.planotrabalho where ptoid=".$linha['ptoordemprov'];
    	$ordem = $db->pegaUm($sql);
    	$sql = "update monitora.planotrabalho set ptostatus='J' , ptoid_antecessor = $ordem where ptoid = ".$linha['id'];
    	$db->executar($sql);
    	
    }
	$db->commit();
}
   
include APPRAIZ."includes/cabecalho.inc";
print '<br>';
$db->cria_aba($abacod_tela,$url,'');
if ($_REQUEST['acao']=='I') $titulo_modulo='Importa CSV';

monta_titulo($titulo_modulo,'');


?>

<form method="POST"  name="formulario">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name='act' value="0">
<input type='hidden' name='arquivo' value='0'>
<center>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='right' class="subtitulodireita">Denominação:</td>
        <td><?=$pjecod.'.'.$pjedsc?></td>
      </tr>
  <tr>
  <td></td>
  <td>
  <input type="button" name="btequipe" value="Importa" onclick="importa()">
  </td>
  </tr>     

</table>

</form>



<script>
function importa()
{
		document.formulario.arquivo.value = 'Simec_final_exportacao.csv';
		document.formulario.act.value = '2';
		document.formulario.submit();
}

</script>
