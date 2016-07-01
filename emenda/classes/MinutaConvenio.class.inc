<?php

class MinutaConvenio{
	public $db;
	public $alias;
	public $ptrid;
	public $cpfPresidente;
	
	public function __construct( $ptrid){
		global $db;
		if($db) $this->db = $db; 
		else $this->db = new cls_banco();
		$this->alias = '';
		$this->ptrid = $ptrid;
		$this->cpfPresidenteSubs =  ;
		$this->cpfPresidente =  $this->db->pegaUm("SELECT e.entnumcpfcnpj FROM emenda.concedente c 
													inner join entidade.entidade e on e.entid = c.entid and c.convigente = 't'");// '';
	}
	public function alteraMacrosConvenio(&$imitexto, $pmcid) {
		
		$sql = "SELECT
					micid,
				  	miccampo,
				  	mictabela,
				  	miccoluna,
				  	micschema,
				  	micdescricao
				FROM
					emenda.minutacampos
				WHERE
					micstatus = 'A'";

		$dados = $this->db->carregar($sql);
		$dados = $dados ? $dados : array();
		
		foreach ($dados as $key => $v) {
			$this->alias = '';
			if(strpos($imitexto, $v["miccampo"])) {
				
				if($v["miccampo"] == '#Tabela_Cronograma_Desembolso#'){
					//monta a tabela cronograma desembolso
					$varAlteracaoMacro = $this->montaTabelaParcelaDesembolso();
				} else if($v["miccampo"] == '#Tabela_Classe_Financeira#'){
					//monta a tabela de execução financeira
					$varAlteracaoMacro = $this->montaTabelaClasseFinanceira();
				} else {
					/*caso contrario substitui os dados pelo o retorno do sql*/
					// monta o 'select' para realizar a substituição			
					$sql1 = $this->montaSQLMacroConvenio($v['mictabela'], $v['miccampo'], $v['miccoluna']);
					
					if( strtolower( $v['miccampo']) == strtolower('#Novo_Valor_Convenio#') ){
						$sql1 = "SELECT sum(exe.exfvalor) FROM emenda.planotrabalho ptr 
								    inner join emenda.execucaofinanceira exe on exe.ptrid = ptr.ptrid and exe.exfstatus = 'A'
								 WHERE ptr.ptrid = ".$this->ptrid."
								 	and exe.semid = 4";
					}					
					if( strtolower( $v['miccampo']) == strtolower('#Novo_Valor_FNDE#') ){
						$sql1 = "SELECT  sum(pede.pedvalor) as pedvalor
							FROM emenda.planotrabalho ptr 
								inner join emenda.ptemendadetalheentidade pede on pede.ptrid = ptr.ptrid
							    inner join emenda.v_emendadetalheentidade vede on vede.edeid = pede.edeid
							WHERE ptr.ptrid = ".$this->ptrid." and vede.emetipo = 'X'";
					}
					if( strtolower( $v['miccampo']) == strtolower('#Soma_Covenio_Original_Reformulacao#') ){
						$ptrcod = $this->db->pegaUm( "SELECT ptrcod FROM emenda.planotrabalho WHERE ptrid = ".$this->ptrid );
						$sql1 = "SELECT sum(exe.exfvalor) FROM emenda.planotrabalho ptr 
								  	inner join emenda.execucaofinanceira exe on exe.ptrid = ptr.ptrid and exe.exfstatus = 'A'
								WHERE ptr.ptrcod = $ptrcod
									and (ptr.ptridpai is null or ptr.ptrsituacao = 'C')
								  	and exe.semid = 4";
					}
					if(strtolower($v['miccampo']) == strtolower('#Objeto_Convenio#')){
						$varAlteracao = $this->db->carregarColuna($sql1);
						$varMacro = array();
						foreach ($varAlteracao as $objeto) {
							if( strpos( strtolower($objeto), '#numero_convenio#' ) ){
								$sql = "select ptrnumconvenio from emenda.planotrabalho where ptrid = ".$this->ptrid;
								$objeto = str_replace('#Numero_Convenio#', $this->db->pegaUm($sql), $objeto);
							}
							array_push( $varMacro, $objeto );
						}
						$varAlteracaoMacro = implode(', ', $varMacro);
					} else {
						if( $v['miccampo'] != '#Nome_Interveniente#' ) $varAlteracaoMacro = $this->db->pegaUm($sql1);
					}
				}
				
				$arInter = $this->verificaExisteInterveniente( $pmcid );				
				$arInter = $arInter ? $arInter : array();
				
				if( is_array($arInter) && $arInter[0] ){
					$arEntidadeInter = array();
					$arEntidadeDirig = array();
					foreach ($arInter as $valor) {
						$arEntidadeInter[] = $valor['entidinterveniente'];	
						$arEntidadeDirig[] = $valor['entiddirigente'];	
					}
					
					$sql = "SELECT miccampo FROM emenda.minutacampos WHERE micdescricao ilike '%interveniente%'";				
					$arCamposInterveniente = $this->db->carregarColuna($sql);
					
					if( in_array( $v["miccampo"], $arCamposInterveniente ) ){
						$arInterveniente = $this->retornaDadosInterveniente($this->alias.".".$v["miccoluna"], $arEntidadeInter);
						if( strpos( strtolower($v["miccampo"]), 'cnpj') ){
							$arInterveniente = formatar_cpf_cnpj( $arInterveniente );
						}
						$imitexto = str_replace($v["miccampo"], strtoupper(implode(", ", $arInterveniente)), $imitexto);
					}
				}
				/*if( strtolower($v["miccampo"]) == strtolower('#Municipio_Entidade#') ){
					ver( $sql1, $varAlteracaoMacro,d );
				}*/
				
				//verifica se na coluna miccampo existe a string concedente e se o esquema e diferente de emenda.
				//esta verificação e necessária porque existe string concedente na coluna miccampo mas pertence ao esquema emenda
				//e essa verificação e para os valores dos concedente referente a entidade, no caso o presidente do FNDE.
				/*if( strpos( strtolower($v["miccampo"]), 'concedente') && (strtolower($v["micschema"]) != 'emenda') && ($v['miccampo'] == '#Nome_Concedente#') ){
					//$arDadosConcedente = $this->retornaDadosConcedente($this->alias.".".$v["miccoluna"], $this->cpfPresidente);
					if( strpos( strtolower($v["miccampo"]), 'cnpj') || strpos( strtolower($v["miccampo"]), 'cpf') ){
						$arDadosConcedente = formatar_cpf_cnpj( $varAlteracaoMacro );
					}
					$imitexto = str_replace($v["miccampo"], strtoupper($varAlteracaoMacro), $imitexto);
				} else*/ 
				if( !strpos( strtolower($v["miccampo"]), 'interveniente') ){
					if( strpos( strtolower($v["miccampo"]), 'cnpj') || strpos( strtolower($v["miccampo"]), 'cpf') ){
						$varAlteracaoMacro = formatar_cpf_cnpj( $varAlteracaoMacro );
					}
					if( strpos( strtolower($v["miccampo"]), 'cep') ){
						if($varAlteracaoMacro){
							$varAlteracaoMacro = substr($varAlteracaoMacro,0,5) . "-" .
												 substr($varAlteracaoMacro,5,3);
						}
					}
					if( $v["miccampo"] == '#Numero_Processo#' ){ //formata o numero do processo empenho
						if($varAlteracaoMacro){
							$varAlteracaoMacro = substr($varAlteracaoMacro,0,5) . "." .
												 substr($varAlteracaoMacro,5,6) . "/" .
												 substr($varAlteracaoMacro,11,4) . "-" .
												 substr($varAlteracaoMacro,15,2);
						}
					}
					
					if( strpos( strtolower($v["miccampo"]), 'valor') || strpos( strtolower($v["miccampo"]), 'soma') ){
						/*
						 * Alteração feita por Felipe Carvalho (22/12/2009)
						 * A pedido da Priscila, os macrocampos testados abaixo devem vir com seus valores exibidos por extenso. 
						 */ 
						if((integer)$v["micid"] == 120 || (integer)$v["micid"] == 121 || (integer)$v["micid"] == 122 || (integer)$v["micid"] == 126 || (integer)$v["micid"] == 125)
							$varAlteracaoMacro = valorMonetarioExtenso($varAlteracaoMacro);
						else
							$varAlteracaoMacro = number_format( $varAlteracaoMacro, 2, ',', '.' );
					}			
					$imitexto = str_replace($v["miccampo"], strtoupper($varAlteracaoMacro), $imitexto);
				}
			}
		}
	}
	
	public function alteraMacrosConvenioDOU(&$imitexto) {
		
		$sql = "SELECT
					micid,
				  	miccampo,
				  	mictabela,
				  	miccoluna,
				  	micschema,
				  	micdescricao
				FROM
					emenda.minutacampos
				WHERE
					micstatus = 'A'";
		
		$dados = $this->db->carregar($sql);
		$dados = $dados ? $dados : array();
		foreach ($dados as $key => $v) {
			$this->alias = '';
			if(strpos($imitexto, $v["miccampo"])) {
				
				if($v["miccampo"] == '#Tabela_Cronograma_Desembolso#'){
					//monta a tabela cronograma desembolso
					$varAlteracaoMacro = $this->montaTabelaParcelaDesembolso();
				} else if($v["miccampo"] == '#Classe_Financeira#'){
					//monta a tabela de execução financeira
					$varAlteracaoMacro = $this->montaTabelaClasseFinanceira('DOU');
				} else {
					/*caso contrario substitui os dados pelo o retorno do sql*/
					// monta o 'select' para realizar a substituição
					$sql = $this->montaSQLMacroConvenio($v['mictabela'], $v['miccampo'], $v['miccoluna']);
					if(strtolower($v['miccampo']) == strtolower('#Objeto_Convenio#')){
						$varAlteracaoMacro = $this->db->carregarColuna($sql);
						$varAlteracaoMacro = implode(', ', $varAlteracaoMacro);
					} else if( (strtolower($v['miccampo']) == '#cpf_presidente_substituto#') || strtolower($v['miccampo']) == '#nome_presidente_substituto#' ){
						$arDadosConcedente = $this->retornaDadosConcedente($this->alias.".".$v["miccoluna"], $this->cpfPresidenteSubs);
						$varAlteracaoMacro = $arDadosConcedente;
					} else if( strpos( strtolower($v["miccampo"]), 'concedente') && (strtolower($v["micschema"]) != 'emenda') && ($v['miccampo'] == '#Nome_Concedente#') ){
						$arDadosConcedente = $this->retornaDadosConcedente($this->alias.".".$v["miccoluna"], $this->cpfPresidente);
						$varAlteracaoMacro = $arDadosConcedente;
					} else {
						$varAlteracaoMacro = $this->db->pegaUm($sql);
					}
				}
				$sql = "SELECT
							icon.entiddirigente,
						    icon.entidinterveniente
						FROM
							emenda.ptminutaconvenio ptm
						    inner join emenda.intervenienteconvenio icon
						    	on icon.pmcid = ptm.pmcid
						WHERE 
							ptm.ptrid = ".$this->ptrid."
							and ptm.pmcstatus = 'A'
						    and icon.itcstatus = 'A'
						LIMIT 1";
				
				$arInter = $this->db->carregar($sql);
				$arInter = $arInter ? $arInter : array();
				
				if( $arInter ){
					$arEntidadeInter = array();
					$arEntidadeDirig = array();
					foreach ($arInter as $valor) {
						$arEntidadeInter[] = $valor['entidinterveniente'];	
						$arEntidadeDirig[] = $valor['entiddirigente'];	
					}
					
					$sql = "SELECT miccampo FROM emenda.minutacampos WHERE micdescricao ilike '%interveniente%'";				
					$arCamposInterveniente = $this->db->carregarColuna($sql);
					
					if( in_array( $v["miccampo"], $arCamposInterveniente ) ){
						$arInterveniente = $this->retornaDadosInterveniente($this->alias.".".$v["miccoluna"], $arEntidadeInter);
	
						if( strpos( strtolower($v["miccampo"]), 'cnpj') ){
							$arInterveniente = formatar_cpf_cnpj( $arInterveniente );
						}
						
						$varAlteracaoMacro  = strtoupper(implode(", ", $arInterveniente));
					}
				}
				
				if( strpos( strtolower($v["miccampo"]), 'cnpj') || strpos( strtolower($v["miccampo"]), 'cpf') ){
					$varAlteracaoMacro = formatar_cpf_cnpj( $varAlteracaoMacro );
				}
				if( $v["miccampo"] == '#Numero_Processo#' ){ //formata o numero do processo empenho
					if($varAlteracaoMacro){
						$varAlteracaoMacro = substr($varAlteracaoMacro,0,5) . "." .
											 substr($varAlteracaoMacro,5,6) . "/" .
											 substr($varAlteracaoMacro,11,4) . "-" .
											 substr($varAlteracaoMacro,15,2);
					}
				}
				
				if( strpos( strtolower($v["miccampo"]), 'valor') ){
					$varAlteracaoMacro = number_format( $varAlteracaoMacro, 2, ',', '.' );
				}
				$imitexto = str_replace($v["miccampo"], strtoupper($varAlteracaoMacro), $imitexto);
			}
		}
	}
	
	public function montaSQLMacroConvenio($mictabela, $miccampo, $miccoluna){
	
		switch($mictabela) {
			case 'entidade':
				if( strpos( strtolower($miccampo), strtolower('Dirigente') ) ){
					$join =	"INNER JOIN entidade.entidade ent ON ent.entid = ptr.entiddirigente ";
				} else if( strpos( strtolower($miccampo), strtolower('Concedente') ) ){
					$join =	"INNER JOIN entidade.entidade ent ON ent.entid = con.entid ";
				} else{
					if( $miccoluna == 'entnome' ) $miccoluna = 'enbnome';
					if( $miccoluna == 'entnumcpfcnpj' ) $miccoluna = 'enbcnpj';
					$join = "INNER JOIN emenda.entidadebeneficiada ent ON ent.enbid = ptr.enbid ";
				}
				$this->alias =	"ent";
				break;
			case 'entidadebeneficiada':
				if( $miccoluna == 'entnome' ) $miccoluna = 'enbnome';
				if( $miccoluna == 'entnumcpfcnpj' ) $miccoluna = 'enbcnpj';
				$join = "INNER JOIN emenda.entidadebeneficiada ent ON ent.enbid = ptr.enbid ";
				$this->alias =	"ent";
				break;
			
			case 'endereco':
				if( strpos( strtolower($miccampo), strtolower('Dirigente') ) ){
					$join = "INNER JOIN entidade.endereco ende ON ende.entid = ptr.entiddirigente ";
				} else if( strpos( strtolower($miccampo), strtolower('Concedente') ) ){
					$join =	"INNER JOIN entidade.endereco ende ON ende.entid = con.entid ";
				} else{
					if( $miccoluna == 'entnome' ) $miccoluna = 'enbnome';
					if( $miccoluna == 'entnumcpfcnpj' ) $miccoluna = 'enbcnpj';
					$join = "INNER JOIN emenda.entidadebeneficiada ende ON ende.enbid = ptr.enbid ";
				}
				$this->alias =	"ende";
				break;
				
			case 'municipio':
				if( strpos( strtolower($miccampo), strtolower('Dirigente') ) ){
					$join =	"INNER JOIN entidade.endereco ende ON ende.entid = ptr.entiddirigente
							 INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod ";
				} else if( strpos( strtolower($miccampo), strtolower('Concedente') ) ){
					$join =	"INNER JOIN entidade.endereco ende ON ende.entid = con.entid
							 INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod ";
				} else{
					if( $miccoluna == 'entnome' ) $miccoluna = 'enbnome';
					if( $miccoluna == 'entnumcpfcnpj' ) $miccoluna = 'enbcnpj';
					$join =	"INNER JOIN emenda.entidadebeneficiada ende ON ende.enbid = ptr.enbid 
							 INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod ";
				}
				$this->alias =	"mun";
				break;
				
			case 'planotrabalho':
				$join =	"";
				$this->alias =	"ptr";
				break;
			case 'execucaofinanceira':
				$join =	"INNER JOIN emenda.execucaofinanceira exe on exe.ptrid = ptr.ptrid and exe.exfstatus = 'A'";
				$this->alias	=	"exe";
				break;
			case 'v_funcionalprogramatica':
				$join =	"INNER JOIN emenda.ptemendadetalheentidade pede
						    	on pede.ptrid = ptr.ptrid
						    INNER JOIN emenda.v_emendadetalheentidade vede 
						    	on vede.edeid = pede.edeid
						        and vede.edestatus = 'A'
						    INNER JOIN emenda.v_funcionalprogramatica vfun
						    	on vfun.acaid = vede.acaid
						        and vfun.acastatus = 'A'";
				$this->alias = "vfun";
				break;
			case 'ptminutaconvenio':
				$join = "INNER JOIN emenda.ptminutaconvenio pmc on pmc.ptrid = ptr.ptrid and pmc.pmcstatus = 'A'";
				$this->alias =	"pmc";
				break;
			case 'ptvigencia':
				$filtro = "and ptv.vigtipo = 'P'";
				if( strtolower($miccampo) == strtolower('#Numero_Dias_Prorrogados#') || strtolower($miccampo) == strtolower('#Nova_Vigencia_Inicio#') ||
						strtolower($miccampo) == strtolower('#Nova_Vigencia_Fim#') ){
					$filtro = "and ptv.vigtipo IN ('T','O','V')";
				}
				$join = "INNER JOIN emenda.ptvigencia ptv on ptv.ptrid = ptr.ptrid and ptv.vigstatus = 'A' $filtro";
				$this->alias =	"ptv";
				break;
			case 'ptiniciativa':
				$join = "INNER JOIN emenda.ptiniciativa pti on pti.ptrid = ptr.ptrid";
				$this->alias = "pti";
				break;
			case 'iniciativa':
				$join = "INNER JOIN emenda.ptiniciativa pti on pti.ptrid = ptr.ptrid
							 INNER JOIN emenda.iniciativa ini on ini.iniid = pti.iniid and ini.inistatus = 'A'";
				$this->alias =	"ini";
				break;
			case 'objetoconvenio':
				$join = "INNER JOIN emenda.ptminutaconvenio ptm on ptm.ptrid = ptr.ptrid
							INNER JOIN emenda.objetominutaconvenio omc on omc.pmcid = ptm.pmcid
							INNER JOIN emenda.objetoconvenio obc on obc.obcid = omc.obcid";
				$this->alias =	"obc";
				break;
			case 'ptparceladesembolso':
				$join =	"INNER JOIN emenda.ptparceladesembolso ppd on ppd.ptrid = ptr.ptrid and ppd.prdminuta = 'M'";
				$this->alias =	"ppd";
				$distinct = 'DISTINCT';
				break;
			case 'ptparcelainiciativa':
				$join =	"INNER JOIN emenda.ptparceladesembolso ppd on ppd.ptrid = ptr.ptrid
							 INNER JOIN emenda.ptparcelainiciativa ppi on ppi.prdid = ppd.prdid";
				$this->alias =	"ppi";
				break;
				
			case 'funcao':
				if( strpos( strtolower($miccampo), strtolower('Dirigente') ) ){
					$entid = 'ent.entid = ptr.entiddirigente';
					
					$join = "inner join entidade.entidade ent
								on $entid
							inner join emenda.cargos car
						    	on ptr.carid = car.carid
						    inner join entidade.funcao fuc
						    	on fuc.funid = car.funid
						        and fuc.funstatus = 'A'";
					
				}else if( strpos( strtolower($miccampo), strtolower('Concedente') ) ){
					$entid = "ent.entnumcpfcnpj = '$this->cpfPresidente'";
					$join = "inner join entidade.entidade ent
								on $entid
						    inner join entidade.funcaoentidade fue
						    	on fue.entid = ent.entid
						        and fue.fuestatus = 'A'
						        and ent.entstatus = 'A'
						    inner join entidade.funcao fuc
						    	on fuc.funid = fue.funid
						        and fuc.funstatus = 'A'";
				}				
				$this->alias = 'fuc';
				break;
			case 'ptiniciativaespecificacao':
				if( strtolower($miccampo) == strtolower('#Total_Onibus_Transporte_Escolar#') ){
					$join = "inner join emenda.ptiniciativa pti on pti.ptrid = ptr.ptrid and pti.iniid = 14
							 inner join emenda.ptiniciativaespecificacao pte on pte.ptiid = pti.ptiid and pte.ptestatus = 'A'";
					$this->alias =	"pte";
				}				
				break;
			case 'ptemendadetalheentidade':
				$join = "inner join emenda.ptemendadetalheentidade pede on pede.ptrid = ptr.ptrid";
				$this->alias =	"pede";
				break;
			case 'banco':
				$join = "inner join emenda.banco ban on ban.bcoid = ptr.bcoid";
				$this->alias =	"ban";
				break;
			case 'contacorrente':
				$join = "inner join emenda.contacorrente cc on cc.ptrid = ptr.ptrid";
				$this->alias =	"cc";
				break;
			case 'bancoagencia':
				$join = "inner join emenda.banco ban on ban.bcoid = ptr.bcoid
						 inner join financeiro.bancoagencia bco on ban.bcocod = bco.codbanco 
								and bco.codagencia = substr(ptr.ptragenciabancaria, 1,4)";
				$this->alias =	"bco";
				break;
		}
		
/*Add */if( $miccampo == '#Valor_Total_Convenio#' || $miccampo == '#Valor_Total_Convenio_Extenso#'){ //Valor total do convenio é a soma do dois valores: concedente e proponente								
			$sql = "SELECT 
						sum(per.pervalor) + ( select sum(ptrvalorproponente)  from emenda.planotrabalho where ptrid = $this->ptrid ) as pervalor 
					FROM 
						emenda.ptiniciativa pti
					inner join 
						emenda.ptiniciativaespecificacao pte on pte.ptiid = pti.ptiid and pte.ptestatus = 'A'
					inner join 
						emenda.ptiniesprecurso per on per.pteid = pte.pteid
					WHERE 
					    pti.ptrid = $this->ptrid";
		}  else if( $miccampo == '#Valor_Total_Convenio_Ano#' || $miccampo == '#Valor_Total_Convenio_Ano_Extenso#'){ //Valor total do convenio é a soma do dois valores: concedente e proponente do Ano Corrente								
			$sql 	 = 	"SELECT sum(".$this->alias.".".$miccoluna.") + ptr.ptrvalorproponente FROM emenda.planotrabalho ptr ";
			$sql 	.= 	$join;
			$sql 	.= 	" WHERE ptr.ptrid = ".$this->ptrid." and exfnumempenhooriginal is not null and exfstatus = 'A' GROUP BY ptr.ptrvalorproponente";
/*Add */}  else if( $miccampo == '#Valor_Concedente#' || $miccampo ==  '#Valor_Concedente_Extenso#' ){
			$sql = "SELECT 
						sum(per.pervalor) as pervalor 
					FROM 
						emenda.ptiniciativa pti
					inner join 
						emenda.ptiniciativaespecificacao pte on pte.ptiid = pti.ptiid and pte.ptestatus = 'A'
					inner join 
						emenda.ptiniesprecurso per on per.pteid = pte.pteid
					WHERE 
					    pti.ptrid = $this->ptrid";
		}  else if( $miccampo == '#Valor_Concedente_Ano#' || $miccampo == '#Valor_Concedente_Ano_Extenso#'){
			$sql 	 = 	"SELECT sum(".$this->alias.".".$miccoluna.") FROM emenda.planotrabalho ptr ";
			$sql 	.= 	$join;
			$sql 	.= 	" WHERE ptr.ptrid = ".$this->ptrid." and exfnumempenhooriginal is not null and exfstatus = 'A'";
		} else if ( strtolower($miccoluna) == 'vigdatainicio' ||  strtolower($miccoluna) == 'vigdatafim' ){//formata campo data
			$sql 	= 	"SELECT to_char(".$this->alias.".".$miccoluna.", 'DD/MM/YYYY') FROM emenda.planotrabalho ptr ";
			$sql 	.= 	$join;
			$sql 	.= 	" WHERE ptr.ptrid = ".$this->ptrid." and ptv.vigstatus = 'A'";
		} else if ( strpos( strtolower($miccampo), 'data' ) ){//formata campo data
			$sql 	= 	"SELECT to_char(".$this->alias.".".$miccoluna.", 'DD/MM/YYYY') FROM emenda.planotrabalho ptr ";
			$sql 	.= 	$join;
			$sql 	.= 	" WHERE ptr.ptrid = ".$this->ptrid;
		} else if ( strtolower($miccampo) == strtolower('#Total_Onibus_Transporte_Escolar#') ) {				
			$sql 	= 	"SELECT count( ".$this->alias.".".$miccoluna.") FROM emenda.planotrabalho ptr ";
			$sql 	.= 	$join;
			$sql 	.= 	" WHERE ptr.ptrid = ".$this->ptrid;
		} /*else if(strtolower($miccampo) == strtolower('#Objeto_Convenio#')){
			$sql 	= 	"SELECT count( ".$this->alias.".".$miccoluna.") FROM emenda.planotrabalho ptr ";
			$sql 	.= 	$join;
			$sql 	.= 	" WHERE ptr.ptrid = ".$this->ptrid;
		}*/ else {
			if( strpos(strtolower($miccampo), strtolower('Concedente') ) && (strtolower($v["micschema"]) != 'emenda')  ){
				$sql = "SELECT ".$distinct." ".$this->alias.".".$miccoluna." FROM emenda.concedente con ";
				$sql 	.= 	$join;
				$sql 	.= 	" WHERE con.convigente = 't'"; 
			} else {
				$sql 	= 	"SELECT ".$distinct." ".$this->alias.".".$miccoluna." FROM emenda.planotrabalho ptr ";
				$sql 	.= 	$join;
				$sql 	.= 	" WHERE ptr.ptrid = ".$this->ptrid;	
			}
		}
		return $sql;
	}
	public function montaTabelaClasseFinanceira($tipo = ''){
		
		$sql = "SELECT
					exe.exfnaturezadespesa,
				    exe.exfnumempenhooriginal,
				    to_char(exe.exfdataalteracao, 'DD/MM/YYYY') as exfdataalteracao,
				    to_char(exe.exfdatainclusao, 'DD/MM/YYYY') as exfdataemisao,
				    exe.exfvalor,
				    exe.exfcodfontesiafi,
				    vfun.fupfuncionalprogramatica
				FROM 
					emenda.execucaofinanceira exe
				    inner join emenda.ptemendadetalheentidade pede
				    	on pede.pedid = exe.pedid
				    inner join emenda.v_emendadetalheentidade vede
				    	on vede.edeid = pede.edeid
				        and vede.edestatus = 'A'
				    inner join emenda.v_funcionalprogramatica vfun
				    	on vfun.acaid = vede.acaid
				        and vfun.acastatus = 'A'
				WHERE 
				    exe.ptrid = ".$this->ptrid."
				    and exe.exfnumempenhooriginal is not null
					and exe.exfstatus = 'A'";
				    
		$arDados = $this->db->carregar($sql);
		$arDados = $arDados ? $arDados : array();
		$tabela = '';
		$tabela .= '<p style="text-align: justify; padding-left: 60px;"><span style="font-size: small;">';
		if( $tipo == 'DOU' ){
			foreach ($arDados as $key => $valor) {
				$tabela .= 'Crédito Orçamentário: Programa de Trabalho: '.$valor['fupfuncionalprogramatica'].', Fonte de Recurso: '.$valor['exfcodfontesiafi'].', Natureza da Despesa: '.$valor['exfnaturezadespesa'].', Número do Documento: '.$valor['exfnumempenhooriginal'].', de '.$valor['exfdataalteracao'].' no valor de R$ '.number_format($valor['exfvalor'],2,",",".").'.<br>';
			}			
		} else {
			$tabela = '<table style="width: 851px;" border="1" cellspacing="0" cellpadding="0">
						<tbody>
						<tr style="text-align: center;">
							<td rowspan="2">Programa de Trabalho</td>
							<td rowspan="2">Fonte de Recurso</td>
							<td rowspan="2">Natureza da Despesa</td>
							<td colspan="3">Nota de Empenho</td>
						</tr>
						<tr>
							<td>Número</td>
							<td>Data</td>
							<td>Valor(es) em R$</td>
						</tr>';
			
			foreach ($arDados as $key => $valor) {
				$tabela .= '<tr>';
				$tabela .= '<td>'.$valor['fupfuncionalprogramatica'].'</td>';
				$tabela .= '<td>'.$valor['exfcodfontesiafi'].'</td>';
				$tabela .= '<td>'.$valor['exfnaturezadespesa'].'</td>';
				$tabela .= '<td>'.$valor['exfnumempenhooriginal'].'</td>';
				
				$tabela .= '<td>'.$valor['exfdataemisao'].'</td>';
				//$tabela .= '<td>'.$valor['exfdataalteracao'].'</td>';
				
				$tabela .= '<td>'.number_format($valor['exfvalor'],2,",",".").'</td>';
				$tabela .= '</tr>';
			}
			$tabela .= '</tbody></table>';
			$tabela .= "</span></p>";
		}
		
		return $tabela;
		
	}
	public function montaTabelaParcelaDesembolso(){
		
		$sql = "SELECT DISTINCT
					ini.iniid,
					ini.ininome,
				    ppi.privalor,
					to_char(ppd.prddata, 'MM/YYYY') as prddata
				FROM emenda.planotrabalho ptr 
					INNER JOIN emenda.ptparceladesembolso ppd 
				    	on ppd.ptrid = ptr.ptrid 
				        and ppd.prdminuta = 'M'
				        and ppd.prdtipo = 'C'
				    inner join emenda.ptparcelainiciativa ppi
				    	on ppi.prdid = ppd.prdid 
				    inner join emenda.ptiniciativa pti
				    	on pti.ptiid = ppi.ptiid
				    inner join emenda.iniciativa ini
				    	on ini.iniid = pti.iniid 
				        and ini.inistatus = 'A'
				WHERE 
					ptr.ptrid = ".$this->ptrid."
				ORDER BY 
					ini.ininome, prddata";
					
		$arDados = $this->db->carregar($sql);
		$arDados = $arDados ? $arDados : array();
		
		$arIninome = array();
		foreach ($arDados as $v) {
			$arIninome[$v['iniid']][] = $v['iniid']; 	
		}
			
		$ininomeAnt = "";
		$tabela = '<p style="text-align: justify; padding-left: 60px;"><span style="font-size: small;">';
		$tabela .= '<table style="width: 851px;" border="1" cellspacing="0" cellpadding="0">
					<tbody>
					<tr>
						<td width="60%" valign="top"><p>Finalidade</p></td>
						<td width="10%" valign="top"><p>Parcela</p></td>
						<td width="10%" valign="top"><p>Mês/Ano</p></td>
						<td width="20%" valign="top"><p>Valor(es) em R$</p></td>
					</tr>';
	
		$parcela = 0;
		foreach ($arDados as $key => $valor) {
			
			$parcela ++;
			$tabela .= '<tr>';
			if( $valor['ininome'] != $ininomeAnt ){
				$ininomeAnt = $valor['ininome'];
				$tabela .= '<td rowspan="'.count($arIninome[$valor['iniid']]).'" valign="middle">'.$valor['ininome'].'</td>';
				$parcela = 1;
			}
			$tabela .= '<td>'.($parcela).'</td>';
			$tabela .= '<td>'.$valor['prddata'].'</td>';
			$tabela .= '<td>'.number_format($valor['privalor'],2,",",".").'</td>';
			$tabela .= '</tr>';
		}
		$tabela .= '</tbody></table>';
		$tabela .= "</span></p>";
		
		return $tabela;
	}
	public function verificaExisteInterveniente( $pmcid ){
		
		$sql = "SELECT 
				  entidinterveniente,
				  entiddirigente
				FROM 
				  emenda.intervenienteconvenio
				WHERE
				  pmcid = $pmcid";
	  
		return $this->db->carregar($sql);
		//ver( $this, $teste, d );
	}
	public function retornaDadosInterveniente($coluna, $arEntid){
		
		$sql = "SELECT DISTINCT
				    $coluna
				FROM
					entidade.entidade ent
				    left join entidade.endereco ende
				    	on ende.entid = ent.entid
				    left join territorios.municipio mun
				    	on mun.muncod = ende.muncod
				WHERE
					ent.entid in ('".implode("','", $arEntid)."')";
		
		return $this->db->carregarColuna($sql);
	}
	
	public function retornaDadosConcedente($coluna, $entnumcpfcnpj){ //cpf do presidente do fnde
		
		$sql = "SELECT DISTINCT
				    $coluna
				FROM
					entidade.entidade ent
				    left join entidade.endereco ende
				    	on ende.entid = ent.entid
				    left join territorios.municipio mun
				    	on mun.muncod = ende.muncod
				WHERE
					ent.entnumcpfcnpj = '$entnumcpfcnpj'";
	
		return $this->db->pegaUm($sql);
	}
	
	public function carregaMinutaConvenio( $ptridAnalise, $pmcid = null ){
		
		if( $ptridAnalise || $pmcid ){
			if( $pmcid ){
				$filtro[] = 'ptm.pmcid = '.$pmcid;
			} 
			if( $ptridAnalise ){
				$filtro[] = "ptm.pmcstatus = 'A' and ptm.ptrid = ".$ptridAnalise;
			}
		
			$sql = "SELECT 
			    ptm.pmcid, ptm.ptrid, ptm.pmctexto, ptm.pmcstatus,
			    ptv.vigdatainicio, ptv.vigdatafim, ptv.vigdias,
			    ptm.pmcobjeto, ptp.pubdatapublicacao, ptm.pmcresolucao,
			    ptm.pmcdataconversaosiafi, ptm.pmcnumconveniosiafi,
			    ptm.pmcdtatualizaconveniosiafi, ptp.pubtxtpublicacao,
			    ptm.pmcdataassinatura, ptm.pmcdatarap,
			    ptp.pubpagdou, ptm.mdoid, ptv.vigid, ptp.pubid, ptp.mdoid as moidpublicacao,
			    CASE WHEN (SELECT itc.itcid FROM emenda.intervenienteconvenio itc 
			            			WHERE itc.pmcid = ptm.pmcid 
			                        	and itc.itcstatus = 'A' LIMIT 1 ) is not null THEN 'true' ELSE 'false' END as interveniente
			FROM 
			  emenda.ptminutaconvenio ptm
	          left join emenda.ptvigencia ptv
	          	on ptv.ptrid = ptm.ptrid
	          	AND ptv.vigstatus = 'A' and ptv.vigtipo = 'P'
	          left join emenda.ptpublicacao ptp
	          	on ptp.pmcid = ptm.pmcid
	            and ptp.pubstatus = 'A'
			WHERE
			    ".implode( 'and', $filtro );

			return $this->db->pegaLinha($sql);
		} else {
			return array();
		}
	}
	public function carregaMinutaTermoAditivo( $ptridAnalise, $refid = null ){
		
		if( $ptridAnalise || $refid){			
			if( $refid ){
				$filtro[] = ' ptr.refid = '.$refid;
			} 
			if( $ptridAnalise ){
				$filtro[] = " ptr.refstatus = 'A' and ptr.ptrid = ".$ptridAnalise;
			}
			
			$sql = "select refprorrogacaooficio from emenda.ptminreformulacao  where ptrid = $ptridAnalise";
			$refprorrogacaooficio = $this->db->pegaUm($sql);
			$vigtipo = $refprorrogacaooficio == "N" ? "V" : "O";
			
			$estadoAtual = pegarEstadoAtual( $ptridAnalise );
			
			if( $estadoAtual == EM_PROCESSO_REFORMULADO ){
				$where = "refsituacaoreformulacao = 'F' ". ($filtro ? 'and ' . implode( 'and', $filtro ): '' );
			} else if( $estadoAtual == EM_GERACAO_TERMO_ADITIVO ){
				$where = "refsituacaoreformulacao in ('C', 'F') ". ($filtro ? 'and ' . implode( 'and', $filtro ): '' );
			} else {
				$where = "refsituacaoreformulacao in ('C', 'F') ". ($filtro ? 'and ' . implode( 'and', $filtro ): '' );
			}
			
			$sql = "SELECT 
						ptr.refid, ptr.ptrid, ptr.pmcid, ptr.mdoid, ptr.reftexto, ptr.refstatus, ptr.usucpfinclusao,
						ptr.refdatainclusao, ptr.refsituacao, ptr.refdataalteracao, ptr.usucpfalteracao,
					    ptv.vigdatainicio, ptv.vigdatafim, ptv.vigdias, ptv.vigid
					FROM
					    emenda.ptminreformulacao ptr
						left join emenda.ptvigencia ptv
					    	on ptv.ptrid = ptr.ptrid
					    	and ptv.vigstatus = 'A'
					    	and ptv.vigtipo = '$vigtipo'
					WHERE 
					   ".$where;
			
			return $this->db->pegaLinha($sql);
		} else {
			return array();
		}
	}
	
	public function salvarMinutaTermoAditivo( $boTermoAditivo ){
		$reftexto = simec_htmlspecialchars($boTermoAditivo->texto, ENT_QUOTES);
		
		if( $boTermoAditivo->refid ){
			$sql = "UPDATE emenda.ptminreformulacao SET
						pmcid = {$boTermoAditivo->pmcid},
						mdoid = {$boTermoAditivo->mdoid},
						reftexto = '$reftexto',
						refsituacao = 'T',
						refdataalteracao = now(),
						usucpfalteracao = '".$_SESSION['usucpf']."'
					WHERE 
	  					refid = {$boTermoAditivo->refid}";
			
			$this->db->executar( $sql );
		}
		
		if( !empty($boTermoAditivo->obcid) ){
			$sql = "DELETE FROM emenda.objetominutaconvenio WHERE pmcid = ".$boTermoAditivo->pmcid;
			$this->db->executar( $sql );
			foreach ($boTermoAditivo->obcid as $obcid) {
				$sql = "INSERT INTO emenda.objetominutaconvenio( obcid, pmcid) 
						VALUES ( $obcid, $boTermoAditivo->pmcid)";
				$this->db->executar( $sql );	
			}
		}	
		$this->db->commit();
		$this->db->sucesso('principal/minuta','&tipo=T');	
	}
	
	public function carregaPublicacaoPTA( $ptrid, $pubid = null ){
		if( $ptrid || $pubid){
			
			$pubid = ( $pubid ? ' and ptp.pubid = '.$pubid : "AND ptp.pubstatus = 'A'");
			
			$sql = "SELECT ptm.pmcid, ptp.pubid, ptp.pubtxtpublicacao, ptp.pubdatapublicacao, ptp.pubpagdou, ptp.mdoid, ptp.pubnumportaria, ptr.ptrnumconvenio, ptr.ptranoconvenio
			FROM emenda.ptminutaconvenio ptm
				 inner join emenda.ptpublicacao ptp
	             	on ptp.pmcid = ptm.pmcid
	             inner join emenda.planotrabalho ptr
	             	on ptr.ptrid = ptm.ptrid and ptrstatus = 'A' 
			WHERE 
				ptm.ptrid = ".$ptrid." 
				$pubid
				and ptp.pubtxtpublicacao is not null";

			return $this->db->pegaLinha($sql);
		} else {
			return array();
		}
	}
	
	public function listaPublicacaoPTA( $pmcid, $arPerfil = array() ){
		
		if( $pmcid ){
			$acoes = "'<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"carregaPublicacao('||ptp.pubid||', '||ptp.pmcid||');\" border=\"0\">'";
			if( in_array( CONSULTA_GERAL, $arPerfil ) ) $acoes = "'<img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\">'";
			$sql = "SELECT 
						$acoes as acao,
						td.tpddsc,
					    pmc.pmcnumconveniosiafi,
					    ptp.refid,
					    to_char(ptp.pubdatapublicacao, 'DD/MM/YYYY') as data,
					    ptp.pubpagdou,
					    '<center>'||case when ptp.pubstatus = 'I' then 'InaTivo' else 'Ativo' end||'</center>',
					    to_char(ptp.pubdatainclusao, 'DD/MM/YYYY HH24:MI:SS'),
					    usu.usunome as usunome
					FROM 
					    emenda.ptpublicacao ptp
					    left join emenda.modelosdocumentos mod
					        inner join public.tipodocumento td on td.tpdcod = mod.tpdcod and td.tpdstatus = 'A'
					     on mod.mdoid = ptp.mdoid and mod.mdostatus = 'A'
					    left join seguranca.usuario usu on usu.usucpf = ptp.usucpfinclusao
					    inner join emenda.ptminutaconvenio pmc on pmc.pmcid = ptp.pmcid and pmc.pmcstatus = 'A'
					WHERE
						ptp.pmcid = $pmcid";
			$sql = iconv( "UTF-8", "ISO-8859-1", $sql);
		} else {
			$sql = array();
		}
		
		monta_titulo( '', 'Lista de publicação cadastrada' );
		$cabecalho = array("Ação", "Tipo do Documento", "Nº Convenio SIAFI", "id Reformulação", "Data Publicação", "Página Publicação", "Status", "Data Inclusao", "Usuário Inclusão");
		return $this->db->monta_lista($sql, $cabecalho, 20, 4, 'N','Center','','form', $tamanho, $alinhamento);
	}
	public function listaMinutaConvenioPTA( $ptrid, $arPerfil = array() ){
		
		if( $ptrid ){
			if( $_SESSION['exercicio'] == '2009' ){
				/*$coluna = "(SELECT 
								orb.orbnumordembancaria
							FROM 
							  emenda.ordembancaria orb
							  inner join emenda.execucaofinanceira exe on exe.exfid = orb.exfid
							WHERE
								orb.orbnumsolicitacao is not null
							    and exe.exfstatus = 'A'
			                    and exe.ptrid = pmc.ptrid
			                    and orb.orbnumordembancaria is not null) as ordembancaria,";*/
				$coluna = "(SELECT ord.orbnumordembancaria 
                        	FROM emenda.planotrabalho ptr
                              inner join emenda.ptminutaconvenio pt
                              	on pt.ptrid = ptr.ptrid
                              inner join emenda.execucaofinanceira exf
                              	on exf.ptrid = ptr.ptrid
                              	and exf.exfstatus = 'A'
	                          inner join emenda.ordembancaria ord
                              	on ord.exfid = exf.exfid 
                             WHERE pt.pmcid = pmc.pmcid and pt.pmcstatus = 'A' and ord.orbnumordembancaria is not null limit 1 ) as ordembancaria,";
				
				$cabecalho = array("Ação", "Código", "Status", "Documento", "Nº Covênio Siaf", "Nº Ordem bancária", "Data Inclusão", "Usuário Inclusão", "Data Alteração", "Usuário Alteração", "Justificativa Cancel", "Usuario Cancel", "Data Cancel");
			} else {
				$cabecalho = array("Ação", "Código", "Status", "Documento", "Nº Covênio Siaf", "Data Inclusão", "Usuário Inclusão", "Data Alteração", "Usuário Alteração", "Justificativa Cancel", "Usuario Cancel", "Data Cancel");
			}
			
			if( in_array( CONSULTA_GERAL, $arPerfil ) || !verificaPermissaoPerfil( 'analise', 'boolean' ) ){
				$acoes = "'<img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\"> '";
			} else {
				$acoes = "'<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"carregaMinutaConvenio('||pmc.pmcid||');\" border=\"0\"> ' ||
						'<img src=\"../imagens/print.png\" style=\"cursor:pointer;\" title=\"imprimir\" onclick=\"imprimirMinutaPTA('||pmc.pmcid||');\" border=\"0\">'";
			}
			
			$sql = "SELECT
						/*case when pmc.pmcstatus = 'A' then
							case when pmc.pmcdatacancelamento is not null then
			            		'<center><input type=\"checkbox\" id=\"pmcid[' || pmc.pmcid || ']\" name=\"pmcid[]\" value=\"' || pmc.pmcid || '\" disabled=\"disabled\" checked /></center>'
			            	else
			                	'<center><input type=\"checkbox\" id=\"pmcid[' || pmc.pmcid || ']\" name=\"pmcid[]\" value=\"' || pmc.pmcid ||'_'|| pmc.ptrid || '\" /></center>'
			                end 
		                else '' end ||*/
		                $acoes as acao,
						pmc.pmcid,
					    case when pmc.pmcstatus = 'I' then 'Inativo' else 'Ativo' end as status,
					    mod.mdonome,
					    pmc.pmcnumconveniosiafi,
					    $coluna
					    to_char(pmc.pmcdatainclusao, 'DD/MM/YYYY HH24:MI:SS') as datainclui,
					    usu.usunome as nomeinclui,
					    to_char(pmc.pmcdataalteracao, 'DD/MM/YYYY HH24:MI:SS'),
					    usu1.usunome,
					    pmc.pmcjustcancelamento,
					    usu2.usunome as usucancel,
					    to_char(pmc.pmcdatacancelamento, 'DD/MM/YYYY HH24:MI:SS') as datacancel
					FROM
						emenda.ptminutaconvenio pmc
					    left join emenda.modelosdocumentos mod
					    	on mod.mdoid = pmc.mdoid
					        and mod.mdostatus = 'A'
					    left join seguranca.usuario usu
					    	on usu.usucpf = pmc.usucpfinclusao
					        --and usu.usustatus = 'A'
					    left join seguranca.usuario usu1
					    	on usu1.usucpf = pmc.usucpfalteracao
					        --and usu1.usustatus = 'A'
					    left join seguranca.usuario usu2
    						on usu2.usucpf = pmc.usucpfcancelamento
					WHERE
						pmc.ptrid = $ptrid
					order by status, pmc.pmcid";
		} else {
			$sql = array();
		}
		monta_titulo( '', 'Lista de minuta convênio cadastrada' );
		return $this->db->monta_lista($sql, $cabecalho, 20, 4, 'N','Center','','form');
	}
	public function listaMinutaTermoAditivoPTA( $ptrid, $arPerfil = array() ){
		
		if( $ptrid ){
			if( in_array( CONSULTA_GERAL, $arPerfil ) || !verificaPermissaoPerfil('analise', 'boolean') ){
				$acoes = "'<img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\"> '";
			} else {
				$acoes = "'<center><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"carregaTermoAditivo('||ptr.refid||', '||p.ptrid||');\" border=\"0\"> '||
                        '<img src=\"../imagens/print.png\" style=\"cursor:pointer;\" title=\"imprimir\" onclick=\"imprimirMinutaTermoPTA('||ptr.refid||');\" border=\"0\"></center>'";
			}
			$sql = "SELECT
						$acoes as acao, 
						ptr.refid,
					    case when ptr.refstatus = 'I' then 'Inativo' else 'Ativo' end as status,
					    case when ptr.refsituacao = 'A' then 'Análise' else 'Geração do Termo Aditivo' end as situacao,
					    mod.mdonome,
					    to_char(ptr.refdatainclusao, 'DD/MM/YYYY HH24:MI:SS') as refdatainclusao,
					    usu.usunome as usuario,
					    to_char(ptr.refdataalteracao, 'DD/MM/YYYY HH24:MI:SS') as refdataalteracao,
					    usu1.usunome,
					    ptr.ptrid,
					    p.ptridpai
					FROM
						emenda.ptminreformulacao ptr
                        inner join emenda.planotrabalho p on p.ptrid = ptr.ptrid --and p.refid = ptr.refid
					    left join emenda.modelosdocumentos mod on mod.mdoid = ptr.mdoid and mod.mdostatus = 'A'
					    left join seguranca.usuario usu on usu.usucpf = ptr.usucpfinclusao --and usu.usustatus = 'A'
					    left join seguranca.usuario usu1 on usu1.usucpf = ptr.usucpfalteracao --and usu1.usustatus = 'A'
					WHERE
						--ptr.ptrid = $ptrid
						ptr.ptrid in (SELECT ptrid FROM emenda.planotrabalho WHERE ptrcod = (SELECT ptrcod FROM emenda.planotrabalho WHERE ptrid = ".$ptrid.") )
						and refsituacaoreformulacao <> 'E'
					order by ptr.refid";
			
			$arDados = $this->db->carregar($sql);
			$arDados = $arDados ? $arDados : array();
			$arRegistro = array();
			foreach ($arDados as $key => $v) {
				$arRef = array();
				if( !empty( $v['ptridpai'] ) ){
					$arTipoRef = pegaTipoReformulacao( $v['ptrid'] );					
					
					foreach ($arTipoRef as $val) {
						array_push( $arRef, $val['descricao'] );
					}
					
					$sql = "select * from emenda.reformulatipos a, emenda.tiporeformulacao b where (a.refid = '{$v['refid']}') and (a.trefid = b.trefid)";					    		
		    		if ($arTipos = $this->db->carregar($sql)) {
	
			    		unset($htmlTipos);
			    		foreach ($arTipos as $tipo) {
			    			if ($htmlTipos) $htmlTipos .= ", ";
							$htmlTipos .= "$tipo[trefnome]";
			    		}
		    		}
		    		$tipos = !empty($htmlTipos) ? $htmlTipos : implode(',', $arRef);
				} else {
					$tipos = 'Original';
				}
	    		$arRegistro[$key] = array(
	    								'acao' => $v['acao'],
	    								'refid' => $v['refid'],
	    								'tiporef' => $tipos,
	    								'status' => $v['status'],
	    								'situacao' => $v['situacao'],
	    								'modnome' => $v['mdonome'],
	    								'refdatainclusao' => $v['refdatainclusao'],
	    								'usuario' => $v['usuario'],
	    								'refdataalteracao' => $v['refdataalteracao'],
	    								'usunome' => $v['usunome'],
	    						);
			}
		} else {
			$sql = array();
		}
		
		monta_titulo( '', 'Lista de minuta convênio cadastrada' );
		$cabecalho = array("Ação", "Código", "Tipo Reformulação", "Status", "Situação", "Documento", "Data Inclusão", "Usuário Inclusão", "Data Alteração", "Usuário Alteração");
		return $this->db->monta_lista($arRegistro, $cabecalho, 20, 4, 'N','Center','','form');
	}
}
