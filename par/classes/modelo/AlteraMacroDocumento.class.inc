<?php
class AlteraMacroDocumento{
	public $db;
	public $alias;
	public $muncod;
	
	public function __construct( $muncod ){
		global $db;
		if($db) $this->db = $db; 
		else $this->db = new cls_banco();
		$this->alias = '';
		$this->muncod = $muncod;
	}
	
	public function montaTermo($post = array()){
		
		$sql = "SELECT mdo.mdoconteudo FROM par.modelosdocumentos mdo WHERE mdo.tpdcod = 30 and mdo.mdostatus = 'A'";
		$imitexto = $this->db->pegaUm( $sql );
		
		if($imitexto){
			$this->alteraMacrosDocumento( $imitexto, $post['preid'] );
			
			$campos	= array(
							"resid" => "'{$post['resid']}'",
							"tteid" => TIPO_TERMO,
							"proid" => "123",
							"terdatainclusao" => "'now()'",
							"usucpf" => "'{$_SESSION['usucpf']}'"
							);
						
			$arParam = array( 'tabela' => 'termo',
							  'campo' => $campos,
							  'extensao' => 'pdf',
							  'nome' => 'Termo de Compromisso - PAC'
						 );
			$http = new RequestHttp();
			$http->toPdfSave( $imitexto, $arParam );
			
			foreach ($post['preid'] as $v) {
				$sql = "UPDATE obras.preobra SET terid = (select max(terid) from par.termo) WHERE preid = $v";
				$this->db->executar( $sql );
			}			
			if($this->db->commit()){
				echo "<script>
						alert('Operação realizada com sucesso!');
						window.location.href = window.location;
					</script>";
				exit();
			} else {
				echo "<script>
						alert('Falha na operação!');
						window.location.href = window.location;
					</script>";
				exit();
			}
		}else{
			echo "<script>
					alert('Não existem informações sobre esta minuta.');
					window.location.href = window.location;
				</script>";
			exit();
		}
	}
	
	public function alteraMacrosDocumento(&$imitexto, $preid) {
		
		$sql = "SELECT
					tecid,
				  	teccampo,
				  	tectabela,
				  	teccoluna,
				  	tecschema,
				  	tecdescricao
				FROM
					par.termocampos
				WHERE
					tecstatus = 'A'";
		
		$dados = $this->db->carregar($sql);
		$dados = $dados ? $dados : array();
		
		foreach ($dados as $key => $v) {
			$this->alias = '';
			if(strpos($imitexto, $v["teccampo"])) {
						
				$sql1 = $this->montaSQLMacroConvenio($v['tectabela'], $v['teccampo'], $v['teccoluna']);
				
				$varAlteracaoMacro = $this->db->pegaUm($sql1);
								
				$imitexto = str_replace($v["teccampo"], strtoupper($varAlteracaoMacro), $imitexto);
			}
		}
	}
	
	public function montaSQLMacroConvenio($tectabela, $teccampo, $teccoluna){
	
		switch($tectabela) {
			case 'entidade':
				$join = "INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod 
					     INNER JOIN entidade.endereco ende ON ende.muncod = mun.muncod and ende.endstatus = 'A'
					     INNER JOIN entidade.entidade ent ON ent.entid = ende.entid ";
				$this->alias =	"ent";
				break;
							
			case 'endereco':
				$join = "INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod 
					     INNER JOIN entidade.endereco ende ON ende.muncod = mun.muncod and ende.endstatus = 'A' ";
				$this->alias =	"ende";
				break;
				
			case 'municipio':
				$join =	"INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod";
				$this->alias =	"mun";
				$distinct = 'DISTINCT';
				break;
				
			case 'preobra':
				$join =	"";
				$this->alias =	"pre";
				$distinct = 'DISTINCT';
				break;
				
		}		
		$sql 	= 	"SELECT ".$distinct." ".$this->alias.".".$teccoluna." FROM obras.preobra pre ";
		$sql 	.= 	$join;
		$sql 	.= 	" WHERE pre.muncod = '".$this->muncod."' and pre.prestatus = 'A'";

		return $sql;
	}
}
?>