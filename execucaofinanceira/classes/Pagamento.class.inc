<?php
class Pagamento extends Processo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "execucaofinanceira.empenho";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "empid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'empid' => null, 
									  	'proid' => null, 
									  	'empnumero' => null,
    									'empano' => null,
    									'emppi' => null,
    									'empptres' => null,
    									'empnumeroempenhosigef' => null,
    									'empfonterecurso' => null,
    									'empnaturezadespesa' => null,
    									'empcentrogestaosolic' => null,
    									'empnumeroconvenio' => null,
    									'empanoconvenio' => null,
    									'empgestaoeminente' => null,
    									'empunidadegestoraeminente' => null,
    									'empprogramafnde' => null,
    									'empsituacao' => null,
    									'usucpf' => null,
    									'empprotocolo' => null
									  );

	/*								  
	function __construct($controleMontagemDeTela = NULL) {
		
	}
		*/	

	function __construct() {
		parent::__construct();
	}
	
	public function cabecalhoSolicitacaoEmpenho() {
		global $db;
	
		$arrDados = $db->pegaLinha("SELECT m.muncod,
										   CASE WHEN iu.itrid = 1 THEN e.estuf ELSE m.estuf END as estuf, 
										   m.mundescricao,
										   p.prpnumeroprocesso,
										   p.prptipo
									FROM par.processopar p
									INNER JOIN par.instrumentounidade iu on iu.inuid = p.inuid
								    LEFT JOIN territorios.municipio m ON m.muncod = iu.muncod
									LEFT JOIN territorios.estado e ON e.estuf = iu.estuf
								    WHERE p.prpid='".$_SESSION['execucaofinanceira']['proid']."'
								    AND p.prpstatus = 'A'
								    ");
	
		echo "<table border=0 cellpadding=3 cellspacing=0 class=listagem width=95% align=center>";
		echo "<tr>";
		echo "<td class=SubTituloDireita>UF:</td>";
		echo "<td>".$arrDados['estuf']."</td>";
		echo "</tr>";
		echo "<tr>";
		echo "<td class=SubTituloDireita>Município:</td>";
		echo "<td>".$arrDados['mundescricao']."</td>";
		echo "</tr>";
		echo "<tr>";
		echo "<td class=SubTituloDireita>Nº processo:</td>";
		echo "<td>".$arrDados['prpnumeroprocesso']."</td>";
		echo "</tr>";
		echo "</table>";
	}
}
?>