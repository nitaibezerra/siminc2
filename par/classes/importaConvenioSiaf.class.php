<?php
class importaConvenio{
	private $diretorio;
	private $arquivo;
	
	function __construct(){
		$this->arquivo = null;
		$this->diretorio = null;
	}
	
	function finaliza(){
		$this->arquivo = null;
	}

	function importar( $arq ){
		$this->arquivo = $arq;
		try{
			if( $fh = fopen( $arq, 'r') ){
				
				while($linha = fgets($fh)){
	
					$lines[] = sprintf( "('%s')" , implode( "','" , array(
						trim(substr($linha, 0, 8)),
						trim(substr($linha, 8, 6)),
						trim(substr($linha, 14, 14)),
						trim(substr($linha, 28, 20)),
						trim(substr($linha, 48, 14)), 
						trim(substr($linha, 62, 11)),
						'now()'
						))
					);						
				}
				$this->insereConvenio($lines);
				fclose($fh);
			}
		} catch (Exception $e){
			//ver('Exception');
			//$this->addLog('Erro de Processamento do Arquivo', $e->getMessage(), '');
		}
		$this->finaliza();
	}
	
	private function insereConvenio( $arDados){
		global $db;
		
		if($arDados){
			foreach ($arDados as $value) {
				if($value){					
					$sql = "INSERT INTO 
							  par.dadosconveniosiafi(
							  it_da_transacao,
							  it_nu_convenio,
							  it_co_concedente,
							  it_nu_original,
							  it_co_convenente,
							  it_nu_cpf_responsavel_convenent,
							  dcsdatainclusao
							) 
							VALUES  
							  $value 
							";  
							  
					$db->executar($sql);
				}
			}
			return $db->commit();
		}
	}

	public function carregaRegistroConvenio(){
		global $db;
		$sql = "SELECT 
				  dcsid,
				  it_da_transacao,
				  it_nu_convenio,
				  it_co_concedente,
				  it_nu_original,
				  it_co_convenente,
				  it_nu_cpf_responsavel_convenent,
				  to_char(date(dcsdatainclusao), 'DD/MM/YYYY') as dcsdatainclusao
				FROM 
				  par.dadosconveniosiafi
				WHERE dscstatus = 'A'";
		
		$cabecalho 	= array("Cσdigo", "Data do Arquivo", "Convκnio", "Concedente", "N° Original", "Convenente", "Responsavel", "Data Importaηγo");
		return $db->monta_lista_simples($sql, $cabecalho, 1000000, 10, 'N', '', '', true, '', '', true);
	}
}
?>