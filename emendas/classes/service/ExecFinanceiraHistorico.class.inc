<?php
	
class ExecFinanceiraHistorico extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "emenda.execfinanceirahistorico";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "efhid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'efhid' => null, 
									  	'usucpf' => null, 
									  	'exfid' => null, 
									  	'semid' => null, 
									  	'exfvalor' => null, 
									  	'exfcodmunicipiosiafi' => null, 
									  	'exfespecieempenho' => null, 
									  	'efhiddataalteracao' => null, 
									  	'exfverifsiafi' => null,
									  	'exfverifcadin' => null,
									  );
									  
	public function carregaHistoricoPorExfid($exfid){
		$sql = "SELECT 
					--efh.efhid,
					--efh.usucpf,       
					u.usunome, 
					efh.exfid,
					se.semcod || ' - ' ||se.semdesc as situacao,
					efh.exfvalor,
					CASE WHEN efh.exfespecieempenho = '01' THEN 'Original'
					 WHEN efh.exfespecieempenho = '02' THEN 'Reforço'
					 WHEN efh.exfespecieempenho = '03' THEN 'Anulação'
					END AS especieempenho,
					to_char(efh.efhiddataalteracao, 'DD/MM/YYYY HH24:mi:ss') as efhiddataalteracao,
					CASE WHEN efh.exfverifsiafi = true THEN 'Sim'
					 ELSE 'Não'
					END AS siafi,
					CASE WHEN efh.exfverifcadin = true THEN 'Sim'
					 ELSE 'Não'
					END AS cadin
				 FROM emenda.execfinanceirahistorico efh
					inner join emenda.execucaofinanceira ef on efh.exfid = ef.exfid
					inner join emenda.situacaoempenho se on efh.semid = se.semid 
					inner join seguranca.usuario u on efh.usucpf = u.usucpf
				 WHERE efh.exfid = $exfid";
		$arDados = $this->carregar($sql);
		$arDados = ($arDados) ? $arDados : array();
		return $arDados;
	}
}