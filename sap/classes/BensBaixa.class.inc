<?php
/**
 * Modelo de Baixa de Bens
 * @author Wesley Romualdo da Silva
 */
class BensBaixa extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "sap.bensbaixa";	

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "bebid" );

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
	  	'bebid' => null,
    	'tesid' => null, 
	  	'bebdiploma' => null, 
	  	'bebdata' => null, 
	  	'bebdataprocesso' => null, 
	  	'bebcnpjrecebedor' => null, 
	  	'bebnomerecebedor' => null, 
	  	'bebnumnotasiafi' => null, 
	  	'bebsituacao' => null, 
	  	'bebobs' => null, 
	  	'usucpf' => null, 
	  	'ternum' => null,
	    'bebnumportariacomissao' => null,
	    'bebdtportariacomissao' => null,
	    'bebnumprocesso' => null,
	    'ternumanterior' => null
    
	);
	
	/**
     * Lista bensbaixa através dos filtros informados
     * @name listarBensBaixaPorFiltro
     * @author Wesley Romualdo da Silva
     * @access public
     * @param array post - Dados do formulário
     * @param string $filtroBenStatus - Status do bem (A - ativo, I - inativo)
     * @return void
     */
	public function listarBensBaixaPorFiltro($post, $filtroBenStatus){

		"<a style=\"cursor:pointer;\" onclick=\"if(confirm(\'Deseja excluir o registro?\')){removerBensBaixa(\''||beb.bebid||'\');}\">
			<img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
		</a>";
		
		if(!empty($post['tesid'])) 				$where .= " AND beb.tesid = '".$post['tesid']."' ";
		if(!empty($post['bebdata'])) 			$where .= " AND beb.bebdata = '".formata_data_sql($post['bebdata'])."' ";
		if(!empty($post['bebnumprocesso']))		$where .= " AND beb.bebnumprocesso = '".str_replace( array('.', '-'), '', $post['bebnumprocesso'])."' ";
		if(!empty($post['bebdataprocesso'])) 	$where .= " AND beb.bebdataprocesso = '".formata_data_sql($post['bebdataprocesso'])."' ";
		
		$sql = "SELECT 
					'<center>' || 
						CASE WHEN 
							(bebsituacao = 'I') 
						THEN
							'
							<a style=\"cursor:pointer;\" onclick=\"detalharBensBaixaEmAberto(\''||beb.bebid||'\');\">
								<img src=\"/imagens/editar_nome.gif \" border=0 title=\"Alterar\">
							</a>
							<a style=\"cursor:pointer;\" onclick=\"if(confirm(\'Deseja excluir o registro?\')){removerBensBaixa(\''||beb.bebid||'\');}\">
								<img src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" />
							</a>'
						ELSE
							'<a style=\"cursor:pointer;\" onclick=\"detalharBensBaixa(\''||beb.bebid||'\');\">
								<img src=\"/imagens/editar_nome.gif \" border=0 title=\"Alterar\">
							</a>'
						END 
					|| '
					</center>' as acao,
                    tes.tesdsc,
                    '<span style=\"display:none;\">' || beb.bebdata || '</span>' || to_char(beb.bebdata, 'DD/MM/YYYY') as bebdata,
                    SUBSTR(cast(beb.bebnumprocesso AS text) , 1 , 5)||'.'||SUBSTR(cast(beb.bebnumprocesso AS text) , 6 , 6)||'.'||SUBSTR(cast(beb.bebnumprocesso AS text) , 12 , 4)||'-'||SUBSTR(cast(beb.bebnumprocesso AS text) , 16 , 2) as bebnumprocesso,
                    '<span style=\"display:none;\">' || beb.bebdataprocesso || '</span>' || to_char(beb.bebdataprocesso, 'DD/MM/YYYY') as bebdataprocesso,
                    (SELECT count(*) FROM sap.bensbaixargp WHERE bebid=beb.bebid)||' ' as qtditens,
                    beb.bebnomerecebedor
				FROM 
					sap.bensbaixa beb
				INNER JOIN
					sap.tipoentradasaida tes 
					ON tes.tesid = beb.tesid
				WHERE 
					bebsituacao = '$filtroBenStatus' 
					$where
                ORDER BY 
                	beb.bebdata";
		
		$cabecalho = array("Comando", "Motivo da Baixa", "Data da Baixa", "Número do Processo", "Data do Processo", "Qtd de Itens", "Recebedor");
		$this->monta_lista($sql,$cabecalho,20,50,false,"center",'S','','',array('center','center','center','center','center','center','center'));
		
	}

	/**
     * Lista RGP que serão baixados
     * @name listarBaixaRGP
     * @author Wesley Romualdo da Silva
     * @access public
     * @param int $benid - Id a ser pesquisado
     * @return void
     */
	public function listarBaixaRGP($bebid){
	
		$sql = "SELECT 
					'<center>
						<a style=\"cursor:pointer;\" onclick=\"detalharBensBaixa(\''||bbr.bbrid||'\');\">
							<img src=\"/imagens/editar_nome.gif \" border=0 title=\"Alterar\">
						</a>
						<a style=\"cursor:pointer;\" onclick=\"if(confirm(\'Deseja excluir o RGP da Baixa?\')){removerBensBaixaRGP(\''||bbr.bbrid||'\');}\">
							<img src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" />
						</a>
					</center>' as acao,
					rgp.rgpnum||' ' AS rgpnum,
					mat.matdsc,
					(ecodsc || '/' || mecdsc) estadomotivo
				FROM 
					sap.bensbaixargp bbr
				INNER JOIN 
					sap.bensbaixa beb
					ON beb.bebid=bbr.bebid				
				INNER JOIN 
					sap.rgp rgp
					ON rgp.rgpid=bbr.rgpid
				INNER JOIN 
					sap.bensmaterial bmt 
					ON bmt.bmtid = rgp.bmtid
				INNER JOIN 
					sap.material mat 
					ON mat.matid = bmt.matid
				INNER JOIN
					sap.estadomotivos esm
					ON  esm.esmid=rgp.esmid
				INNER JOIN
					sap.estadoconservacao eco
					ON  eco.ecoid=esm.ecoid
				INNER JOIN
					sap.motivoestadoconservacao mec
					ON  mec.mecid=esm.mecid
				WHERE
					bbr.bebid = $bebid
				ORDER BY 
					mat.matdsc";
		
		$cabecalho = array("Comando","RGP","Material", "Estado/Motivo de Conservação");
		$this->monta_lista($sql,$cabecalho,20,50,false,"center",'S','','',array('center','center','center','center'));
		
	}
	
	/**
	 * Salva novo registro
	 * @name salvar
	 * @author Silas Matheus
	 * @access public
	 * @param array $campos - Campos para inserir
	 * @return int
	 */
	public function salvar($campos){
		
		extract($campos);
		
		//retira os caracteres não validos
		$bebnumprocesso = str_replace(array('.', '-'), '', $bebnumprocesso);
		$bebcnpjrecebedor = str_replace(array('.', '/', '-'), '', $bebcnpjrecebedor);
		
		//formata a data para inserção		
		$bebdata = formata_data_sql($bebdata);
		$bebdataprocesso = formata_data_sql($bebdataprocesso);
		if(!empty($bebdtportariacomissao))
			$bebdtportariacomissao = "'" . formata_data_sql($bebdtportariacomissao) . "'";
		else
			$bebdtportariacomissao = "NULL";

		$sql = "INSERT INTO 
					$this->stNomeTabela
					(tesid, bebdata, bebnumprocesso, bebdataprocesso, bebcnpjrecebedor, 
					bebnomerecebedor, bebnumnotasiafi, bebobs, bebsituacao, bebnumportariacomissao, bebdtportariacomissao)
				VALUES 
					($tesid, '$bebdata', '$bebnumprocesso', '$bebdataprocesso', '$bebcnpjrecebedor', 
					'$bebnomerecebedor', '$bebnumnotasiafi', '$bebobs', 'I', '$bebnumportariacomissao', $bebdtportariacomissao) 
				RETURNING 
					bebid";
					
		
	 	$bebid = $this->pegaUm($sql);
	 	$this->commit(); 
		return $bebid;
	
	}
	
	/**
     * Conclui o cadastro de bensbaixa
     * @name ativar
     * @author Silas Matheus
     * @access public
     * @param int $bebid - Id da tabela bensbaixa
     * @return array
     */
	public function ativar($bebid){
		
		$baixa = $this->pegarRegistro($bebid);
		
		$oTermo = new Termo();
		$ternum = $oTermo->gerarTermo('BA',$baixa['ternum']);
		
		//atualizar o bem para o status ativo
		if(empty($baixa['ternum'])){
			$sql = "UPDATE 
					$this->stNomeTabela
				SET 
					bebsituacao='A',
					ternum=$ternum
				WHERE
					bebid=$bebid";
		}
		else{
			$sql = "UPDATE 
					$this->stNomeTabela
				SET 
					bebsituacao='A',
					ternum=$ternum,
					ternumanterior=".$baixa['ternum']."
				WHERE
					bebid=$bebid";
		}
				
		
		$this->executar($sql);
		
		// atualizar os rgps para o status baixado
		$sql = "UPDATE 
					sap.rgp 
				SET 
					sbmid=" . SBM_BAIXADO . " 
				WHERE 
					rgpid in (select rgpid from sap.bensbaixargp where bebid=$bebid);";
		
		$this->executar($sql);
		
		$this->commit(); 
		
		$oBensHistorico = new BensHistorico(); 
		$oBensHistorico->gravar($_SESSION['just'], $bebid);

	}

	/**
     * Carrega os registros para montar o cabeçalho do relatório de visualização
     * @name cabecalhoVisualizaBensBaixaPadrao
     * @author Alysson Rafael
     * @access public
     * @param int $bebid - Identificador da baixa
     * @return array
     */
	public function cabecalhoVisualizaBensBaixaPadrao($bebid){
		if(!empty($bebid)){
			
			$bebid = addslashes($bebid);
			
			$sql = "SELECT 
					tes.tesdsc,
					TO_CHAR(baixa.bebdata,'DD/MM/YYYY') AS bebdata,
					baixa.bebnumprocesso,
					TO_CHAR(baixa.bebdataprocesso,'DD/MM/YYYY') AS bebdataprocesso,
					CASE 
						WHEN CHAR_LENGTH(CAST(baixa.bebcnpjrecebedor AS text)) = 14 THEN 
							SUBSTR(CAST(baixa.bebcnpjrecebedor AS text) , 1 , 2)||'.'||SUBSTR(CAST(baixa.bebcnpjrecebedor AS text) , 3 , 3)||'.'||SUBSTR(CAST(baixa.bebcnpjrecebedor AS text) , 6 , 3)||'/'||SUBSTR(CAST(baixa.bebcnpjrecebedor AS text) , 9 , 4)||'-'||SUBSTR(CAST(baixa.bebcnpjrecebedor AS text) , 13 , 2) 
						WHEN CHAR_LENGTH(CAST(baixa.bebcnpjrecebedor AS text)) = 11 THEN 
							SUBSTR(CAST(baixa.bebcnpjrecebedor AS text) , 1 , 3)||'.'||SUBSTR(CAST(baixa.bebcnpjrecebedor AS text) , 4 , 3)||'.'||SUBSTR(CAST(baixa.bebcnpjrecebedor AS text) , 7 , 3)||'-'||SUBSTR(CAST(baixa.bebcnpjrecebedor AS text) , 10 , 2) 
						ELSE CAST(baixa.bebcnpjrecebedor AS text) 
					END AS bebcnpjrecebedor, 
					baixa.bebnomerecebedor,
					baixa.bebobs
					FROM sap.bensbaixa baixa
					JOIN sap.tipoentradasaida tes ON baixa.tesid=tes.tesid
					WHERE baixa.bebid='{$bebid}' ";
			
			$arDados = $this->pegaLinha($sql);
			$arDados = $arDados ? $arDados : array();
		} 
		else{
			$arDados = array();
		}
		
		return $arDados;
	}
	
	/**
     * Carrega os registros para montar o cabeçalho do relatório de visualização
     * @name cabecalhoVisualizaBensBaixaSubstituicao
     * @author Alysson Rafael
     * @access public
     * @param int $bebid - Identificador da baixa
     * @return array
     */
	public function cabecalhoVisualizaBensBaixaSubstituicao($bebid){
		if(!empty($bebid)){
			
			$bebid = addslashes($bebid);
			
			$sql = "SELECT 
					tes.tesdsc,
					TO_CHAR(baixa.bebdata,'DD/MM/YYYY') AS bebdata,
					baixa.bebnumprocesso,
					baixa.bebobs
					FROM sap.bensbaixa baixa
					JOIN sap.tipoentradasaida tes ON baixa.tesid=tes.tesid
					WHERE baixa.bebid='{$bebid}' ";
			
			$arDados = $this->pegaLinha($sql);
			$arDados = $arDados ? $arDados : array();
		} 
		else{
			$arDados = array();
		}
		
		return $arDados;
	}
	
	/**
     * Lista de rgps do relatorio de vizualizacao
     * @name listaVisualizaBaixaBens
     * @author Silas Matheus
     * @access public
     * @param int $bebid - Id do registro a ser pesquisado
     * @return void
     */
	public function listaVisualizaBaixaBens($bebid){
		
			$sql = "SELECT 
						r.rgpnum||' ' AS rgpnum,
						ma.matdsc,
						(ecodsc || '/' || mecdsc) estadomotivo,
						bm.bmtvlrunitario
					FROM sap.rgp r
					INNER JOIN 
						sap.bensmaterial bm 
						on bm.bmtid = r.bmtid
					INNER JOIN 
						sap.bens b 
						on b.benid = bm.benid
					INNER JOIN 
						sap.bensbaixargp bbr 
						on bbr.rgpid = r.rgpid
					INNER JOIN 
						sap.material ma 
						on ma.matid = bm.matid
					INNER JOIN
						sap.estadomotivos esm
						ON  esm.esmid=r.esmid
					INNER JOIN
						sap.estadoconservacao eco
						ON  eco.ecoid=esm.ecoid
					INNER JOIN
						sap.motivoestadoconservacao mec
						ON  mec.mecid=esm.mecid
					WHERE
						bbr.bebid=$bebid                  
					ORDER BY ma.matdsc";
		
		$cabecalho = array("RGP","Descrição do Bem", "EC.*", "Valor Unitário");
		$this->monta_lista($sql,$cabecalho,9000,50,true,"center",'S','','',array('center','center','center','right'));
	}
	
	/**
	 * Verifica se registro já existe
	 * @name existe
	 * @author Silas Matheus
	 * @access public
	 * @param array $campos - Campos para verificar
	 * @return bool
	 */	
	public function existe($campos){

		extract($campos);
		
		$bebnumprocesso = str_replace(array('.', '-'), '', $bebnumprocesso);
		
		$sql = "SELECT 
					count(*) count 
				FROM 
					$this->stNomeTabela 
				WHERE 
					bebnumprocesso='$bebnumprocesso'";
	
		if(!empty($campos['bebid']))
			$sql .= "AND bebid<>{$bebid}";		

		$result = $this->carregar($sql);
 	   	$qtd = $result[0]['count'];
		
 	   	if($qtd > 0)
 	   		$resultado = true;

		return $resultado;
		
	}
	
	/**
	 * Recupera registro para edicao
	 * @name pegarRegistro
	 * @author Silas Matheus
	 * @access public
	 * @param int $id - Identificador da situacao
	 * @return array
	 */
	public function pegarRegistro($id){
	
		$id = addslashes($id);
		
		$sql = "SELECT * FROM $this->stNomeTabela WHERE bebid=$id";
		$return = $this->carregar($sql);
		
		return $return[0];
		
	}
	
	/**
	 * Atualiza registro
	 * @name atualizar
	 * @author Silas Matheus
	 * @access public
	 * @param array $campos - Campos que serao atualizados
	 * @return int
	 */
	public function atualizar($campos){
	
		extract($campos);
		
		//retira os caracteres não validos
		$bebnumprocesso = str_replace(array('.', '-'), '', $bebnumprocesso);
		$bebcnpjrecebedor = str_replace(array('.', '/', '-'), '', $bebcnpjrecebedor);
		
		//formata a data para inserção		
		$bebdata = formata_data_sql($bebdata);
		$bebdataprocesso = formata_data_sql($bebdataprocesso);
		if(!empty($bebdtportariacomissao))
			$bebdtportariacomissao = "'" . formata_data_sql($bebdtportariacomissao) . "'";
		else
			$bebdtportariacomissao = "NULL";
		
		$sql = "UPDATE 
					$this->stNomeTabela 
				SET 
					tesid=$tesid, 
					bebnumprocesso='$bebnumprocesso', 
					bebdataprocesso='$bebdataprocesso',
					bebcnpjrecebedor='$bebcnpjrecebedor', 
					bebnomerecebedor='$bebnomerecebedor', 
					bebnumnotasiafi='$bebnumnotasiafi', 
					bebobs='$bebobs',
					bebdtportariacomissao=$bebdtportariacomissao,
					bebnumportariacomissao='$bebnumportariacomissao'
				WHERE 
					bebid='{$campos['bebid']}'				
				RETURNING bebid";
					
		$bebid = $this->pegaUm($sql);
		$this->commit();
		
		$oBensHistorico = new BensHistorico(); 
		$oBensHistorico->gravar($bhsjustificativa, $bebid);
		
		return $bebid;
	
	}
	
	
	
	/**
	 * Atualiza registro
	 * @name atualizarSemHistorico
	 * @author Alysson Rafael
	 * @access public
	 * @param array $campos - Campos que serão atualizados
	 * @return int
	 */
	public function atualizarSemHistorico($campos){
	
		extract($campos);
		
		//retira os caracteres não validos
		$bebnumprocesso = str_replace(array('.', '-'), '', $bebnumprocesso);
		$bebcnpjrecebedor = str_replace(array('.', '/', '-'), '', $bebcnpjrecebedor);
		
		//formata a data para inserção		
		$bebdata = formata_data_sql($bebdata);
		$bebdataprocesso = formata_data_sql($bebdataprocesso);
		if(!empty($bebdtportariacomissao))
			$bebdtportariacomissao = "'" . formata_data_sql($bebdtportariacomissao) . "'";
		else
			$bebdtportariacomissao = "NULL";
		
		$sql = "UPDATE 
					$this->stNomeTabela 
				SET 
					tesid=$tesid, 
					bebnumprocesso='$bebnumprocesso', 
					bebdataprocesso='$bebdataprocesso',
					bebcnpjrecebedor='$bebcnpjrecebedor', 
					bebnomerecebedor='$bebnomerecebedor', 
					bebnumnotasiafi='$bebnumnotasiafi', 
					bebobs='$bebobs',
					bebdtportariacomissao=$bebdtportariacomissao,
					bebnumportariacomissao='$bebnumportariacomissao'
				WHERE 
					bebid='{$campos['bebid']}'				
				RETURNING bebid";
					
		$bebid = $this->pegaUm($sql);
		$this->commit();
		
		return $bebid;
	
	}
	
	
	
	/**
	 * Exclui registro
	 * @name excluir
	 * @author Silas Matheus
	 * @access public
	 * @param int $id - Identificador do Bem
	 * @return bool
	 */
	public function excluir($id){
	
		$id = addslashes($id);
		
			$sql = "SELECT * FROM sap.bensbaixargp WHERE bebid=$id";
			$bbrs = $this->carregar($sql);

			// se tiver filhos
			if(is_array($bbrs) && count($bbrs) > 0){
				$oBensBaixaRgp = new BensBaixaRgp();
				foreach($bbrs as $key=>$bbr){
					$oBensBaixaRgp->excluir($bbr['bbrid']);
				}
			}
			$sql = "DELETE FROM $this->stNomeTabela WHERE bebid=$id";
			$return = $this->executar($sql);
			$this->commit();
		
		return $return;
		
	}
	
	
	
	
	public function salvarPopup($campos){
		
		extract($campos);
		
		//retira os caracteres não validos
		$bebnumprocesso = str_replace(array('.', '-'), '', $bebnumprocesso);
		
		//formata a data para inserção		
		$bebdata = formata_data_sql($bebdata);
		$bebdataprocesso = formata_data_sql($bebdataprocesso);
		

		$sql = "INSERT INTO 
					$this->stNomeTabela
					(tesid, bebdata, bebnumprocesso, bebdataprocesso, bebsituacao)
				VALUES 
					($tesid, '$bebdata', '$bebnumprocesso', '$bebdataprocesso','I') 
				RETURNING 
					bebid";
					
		
	 	$bebid = $this->pegaUm($sql);
	 	$this->commit(); 
		return $bebid;
	
	}
	
	
	
	
	
	public function ativarPopup($bebid){
		
		$baixa = $this->pegarRegistro($bebid);
		
		$oTermo = new Termo();
		$ternum = $oTermo->gerarTermo('BA',$baixa['ternum']);
		
		//atualizar o bem para o status ativo
		if(empty($baixa['ternum'])){
			$sql = "UPDATE 
					$this->stNomeTabela
				SET 
					bebsituacao='A',
					ternum=$ternum
				WHERE
					bebid=$bebid";
		}
		else{
			$sql = "UPDATE 
					$this->stNomeTabela
				SET 
					bebsituacao='A',
					ternum=$ternum,
					ternumanterior=".$baixa['ternum']."
				WHERE
					bebid=$bebid";
		}
				
		
		$this->executar($sql);
		
		// atualizar os rgps para o status baixado
		$sql = "UPDATE 
					sap.rgp 
				SET 
					sbmid=" . SBM_BAIXADO . " 
				WHERE 
					rgpid in (select rgpid from sap.bensbaixargp where bebid=$bebid);";
		
		$this->executar($sql);
		
		$this->commit(); 
		
		return $ternum;


	}
	
	/**
	 * Pega a sigla do tipo de entrada/saída
	 * @name pegaSiglaTipoEntradaSaida
	 * @author Alysson Rafael
	 * @access public
	 * @param int $bebid - Identificador da baixa
	 * @return string
	 */
	public function pegaSiglaTipoEntradaSaida($bebid){
		if(!empty($bebid)){
			
			$bebid = addslashes($bebid);
			
			$sql = "SELECT tipoentsad.tesslg FROM sap.tipoentradasaida tipoentsad ";
			$sql .= "JOIN $this->stNomeTabela baixa ON tipoentsad.tesid=baixa.tesid ";
			$sql .= "WHERE baixa.bebid='{$bebid}' ";
			
			return $this->pegaUm($sql);
		}
	}
	
	/**
	 * Dados do cabeçalho do termo padrão
	 * @name cabecalhoVisualizaTermoPadrao
	 * @author Alysson Rafael
	 * @access public
	 * @param int $bebid - Identificador da baixa
	 * @return array
	 */
	public function cabecalhoVisualizaTermoPadrao($bebid){
		if(!empty($bebid)){

			$bebid = addslashes($bebid);
			
			$sql = "SELECT 
					tes.tesdsc,
					TO_CHAR(baixa.bebdata,'DD/MM/YYYY') AS bebdata,
					termo.ternum,
					TO_CHAR(termo.teremissaodt,'DD/MM/YYYY') AS terdata,
					SUBSTR(cast(baixa.bebnumprocesso AS text) , 1 , 5)||'.'||SUBSTR(cast(baixa.bebnumprocesso AS text) , 6 , 6)||'.'||SUBSTR(cast(baixa.bebnumprocesso AS text) , 12 , 4)||'-'||SUBSTR(cast(baixa.bebnumprocesso AS text) , 16 , 2) AS bebnumprocesso,
					TO_CHAR(baixa.bebdataprocesso,'DD/MM/YYYY') AS bebdataprocesso,
					CASE 
						WHEN CHAR_LENGTH(CAST(baixa.bebcnpjrecebedor AS text)) = 14 THEN 
							SUBSTR(CAST(baixa.bebcnpjrecebedor AS text) , 1 , 2)||'.'||SUBSTR(CAST(baixa.bebcnpjrecebedor AS text) , 3 , 3)||'.'||SUBSTR(CAST(baixa.bebcnpjrecebedor AS text) , 6 , 3)||'/'||SUBSTR(CAST(baixa.bebcnpjrecebedor AS text) , 9 , 4)||'-'||SUBSTR(CAST(baixa.bebcnpjrecebedor AS text) , 13 , 2) 
						WHEN CHAR_LENGTH(CAST(baixa.bebcnpjrecebedor AS text)) = 11 THEN 
							SUBSTR(CAST(baixa.bebcnpjrecebedor AS text) , 1 , 3)||'.'||SUBSTR(CAST(baixa.bebcnpjrecebedor AS text) , 4 , 3)||'.'||SUBSTR(CAST(baixa.bebcnpjrecebedor AS text) , 7 , 3)||'-'||SUBSTR(CAST(baixa.bebcnpjrecebedor AS text) , 10 , 2) 
						ELSE CAST(baixa.bebcnpjrecebedor AS text) 
					END AS bebcnpjrecebedor, 
					baixa.bebnomerecebedor,
					baixa.bebobs,
					baixa.ternumanterior
					FROM sap.bensbaixa baixa
					JOIN sap.tipoentradasaida tes ON baixa.tesid=tes.tesid
					JOIN sap.termo termo ON baixa.ternum=termo.ternum
					WHERE baixa.bebid='{$bebid}' ";
			
			$arDados = $this->pegaLinha($sql);
			$arDados = $arDados ? $arDados : array();
			
		}
		else{
			$arDados = array();
		}
		return $arDados;
	}
	
	/**
	 * Dados do cabeçalho do termo substituição
	 * @name cabecalhoVisualizaTermoSubstituicao
	 * @author Alysson Rafael
	 * @access public
	 * @param int $bebid - Identificador da baixa
	 * @return array
	 */
	public function cabecalhoVisualizaTermoSubstituicao($bebid){
		if(!empty($bebid)){

			$bebid = addslashes($bebid);
			
			$sql = "SELECT 
					tes.tesdsc,
					TO_CHAR(baixa.bebdata,'DD/MM/YYYY') AS bebdata,
					termo.ternum,
					TO_CHAR(termo.teremissaodt,'DD/MM/YYYY') AS terdata,
					SUBSTR(cast(baixa.bebnumprocesso AS text) , 1 , 5)||'.'||SUBSTR(cast(baixa.bebnumprocesso AS text) , 6 , 6)||'.'||SUBSTR(cast(baixa.bebnumprocesso AS text) , 12 , 4)||'-'||SUBSTR(cast(baixa.bebnumprocesso AS text) , 16 , 2) AS bebnumprocesso,
					TO_CHAR(baixa.bebdataprocesso,'DD/MM/YYYY') AS bebdataprocesso,
					baixa.bebobs,
					baixa.ternumanterior
					FROM sap.bensbaixa baixa
					JOIN sap.tipoentradasaida tes ON baixa.tesid=tes.tesid
					JOIN sap.termo termo ON baixa.ternum=termo.ternum
					WHERE baixa.bebid='{$bebid}' ";
			
			$arDados = $this->pegaLinha($sql);
			$arDados = $arDados ? $arDados : array();
			
		}
		else{
			$arDados = array();
		}
		return $arDados;
	}
	
	/**
	 * Carrega as contas contábeis dos itens da baixa
	 * @name listaContaContabilBaixa
	 * @author Alysson Rafael
	 * @access public
	 * @param int $bebid - Identificador da baixa
	 * @return array
	 */
	public function listaContaContabilBaixa($bebid){
		
		if(!empty($bebid)){
			$bebid = addslashes($bebid);
			
			$sql = "SELECT conta.ccbid,conta.ccbdsc
					FROM sap.contacontabil conta
					JOIN sap.itemcontacontabil item ON conta.ccbid=item.ccbid
					JOIN sap.catalogo cat ON item.icbid=cat.icbid AND item.ccbid=cat.ccbid
					JOIN sap.material mat ON cat.catid=mat.catid
					JOIN sap.bensmaterial bemmat ON mat.matid=bemmat.matid
					JOIN sap.rgp rgp ON bemmat.bmtid=rgp.bmtid
					JOIN sap.bensbaixargp bensbaixa ON rgp.rgpid=bensbaixa.rgpid
					JOIN sap.bensbaixa baixa ON bensbaixa.bebid=baixa.bebid
					WHERE baixa.bebid='{$bebid}'
					GROUP BY conta.ccbid,conta.ccbdsc
					ORDER BY conta.ccbid ";
			
			return $this->carregar($sql);
		}
	}
	
	/**
	 * Pega os dados do termo
	 * @name pegaDadosVisualizaTermo
	 * @author Alysson Rafael
	 * @access public
	 * @param int $bebid - Identificador da baixa
	 * @param int $ccbid - Identificador da conta contábil
	 * @return array
	 */
	public function pegaDadosVisualizaTermo($bebid,$ccbid){
		
		if(!empty($bebid) && !empty($ccbid)){
			$bebid = addslashes($bebid);
			$ccbid = addslashes($ccbid);
	
			$sql = "SELECT 
					rgp.rgpnum||' ' AS rgpnum,
					rgp.rgpnumserie||' ' AS rgpnumserie,
					bmt.bmtdscit,
					estcon.ecodsc,
					bmt.bmtvlrunitario
					FROM sap.rgp rgp 
					JOIN sap.bensbaixargp bensbaixa ON rgp.rgpid=bensbaixa.rgpid
					JOIN sap.bensbaixa baixa ON bensbaixa.bebid=baixa.bebid
					JOIN sap.estadomotivos est ON rgp.esmid=est.esmid
					JOIN sap.estadoconservacao estcon ON est.ecoid=estcon.ecoid
					JOIN sap.bensmaterial bmt ON rgp.bmtid=bmt.bmtid 
					JOIN sap.material mat ON bmt.matid=mat.matid 
					JOIN sap.catalogo cat ON mat.catid=cat.catid
					JOIN sap.itemcontacontabil item ON cat.icbid=item.icbid AND cat.ccbid=item.ccbid
					JOIN sap.contacontabil conta ON item.ccbid=conta.ccbid
					WHERE baixa.bebid='{$bebid}' 
					AND conta.ccbid='{$ccbid}'
					ORDER BY rgp.rgpnum ";
			return $this->carregar($sql);
		}
	}

	
}