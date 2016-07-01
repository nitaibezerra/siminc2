<?php

/**
 * Modelo de Termo
 * @author Silas Matheus
 */
class Termo extends Modelo{
	
	/**
	* Nome da tabela especificada
	* @name $stNomeTabela
	* @var string
	* @access protected
	*/
	protected $stNomeTabela = "sap.termo";
	
	/**
	* Chave primária
	* @name $arChavePrimaria
	* @var array
	* @access protected
	*/
	protected $arChavePrimaria = array('ternum');
	
	/**
	* Atributos da Tabela
	* @name $arAtributos
	* @var array
	* @access protected
	*/
	protected $arAtributos = array(
		'ternum' => null,
		'usucpf' => null,
		'tertipo' => null,
		'teremissaodt' => null,
		'ternumanterior' => null
	);

	/**
	 * Salva novo termo
	 * @name gerarTermo
	 * @author Silas Matheus
	 * @access public
	 * @param string(2) $tertipo - Tipo do Termo
	 * @param int $ternumAnterior - número do termo anterior	
	 * @return int
	 */
	public function gerarTermo($tertipo,$ternumAnterior){
	
		$usucpf = $_SESSION['usucpf'];
		
		if(empty($ternumAnterior)){
			$ternumAnterior = 'null';
		}
		
		$sql = "INSERT INTO 
					$this->stNomeTabela 
					(usucpf, tertipo, ternumanterior) 
				VALUES 
					('{$usucpf}','{$tertipo}',{$ternumAnterior}) 
				RETURNING ternum";

		$ternum = $this->pegaUm($sql);
	 	$this->commit(); 
		return $ternum;
	
	}
	
	/**
	 * Pega o termo de baixa
	 * @name carregaTermoBaixa
	 * @author Alysson Rafael
	 * @access public
	 * @param int $bebid - Id da baixa
	 * @return array
	 */
	public function carregaTermoBaixa($bebid){
		if(!empty($bebid)){
			$bebid = addslashes($bebid);
			
			$sql = "SELECT termo.ternum AS ternumbaixa,
					TO_CHAR(termo.teremissaodt,'dd/mm/yyyy') AS terdatabaixa
					FROM $this->stNomeTabela termo
					JOIN sap.bensbaixa baixa ON termo.ternum=baixa.ternum
					WHERE baixa.bebid='{$bebid}' ";
			
			return $this->carregar($sql);
		}
	}
	
	/**
     * Gerar a combo de tipo de termo
     * @name montaComboTipoTermo
     * @author Apoena Machado Cunha
     * @access public
     * @param string $origem - De onde foi chamada a função
     * @param string $selecionado - Item selecionado
     * @return void
     */
	public function montaComboTipoTermo($selecionado='', $origem=''){
		
		//$sqlTipoDoc = "SELECT campo1 AS codigo,campo2 AS descricao FROM sap.tabela ORDER BY campo1 ";
		//$resultadoTipoDoc = $this->carregar($sqlTipoDoc);
		
		// Valores fixos
		//Entrada, Movimentação e Desfazimento
	    $tipos = array( 
	        array('codigo'=>'EN', 'descricao'=>'Entrada'),
	        array('codigo'=>'MV', 'descricao'=>'Movimentação'),
	        array('codigo'=>'BA', 'descricao'=>'Desfazimento')
	    );

		if($origem == 'cadastro'){
			//$this->monta_combo("tdcid",$resultadoTipoDoc,'S',"Todos...","","","","200","S","tdcid","",$tdcid);
		}
		else{
			$this->monta_combo("tertipo",$tipos,'S',"Todos...","ocultaCamposPorTipo","","","200","N","tertipo","", $selecionado);
		}
		
	}
	
	
	/**
     * Lista os dados do termos
     * @name listaTermos
     * @author Apoena Machado Cunha
     * @access public
     * @param array $post - Dados do formulário
     * @return 
     */
	public function listaTermos($post = array()){
		extract($post);
				
		$sqlBase = "
        
        SELECT 
            x.comando
           ,x.ternum
           ,x.responsavel
           ,x.unidade
           ,x.tipo
           ,x.data
           ,x.estado
        
        FROM 
        (
        -- MOVIMENTACAO
        SELECT DISTINCT
           '<center>' ||
           CASE WHEN        			
           	ter.terstatus=0
           THEN 
           	'<img src=\"/imagens/consultar_desabilitado.gif\" border=0 title=\"Termo Inativo\">' 
           ELSE 
              '<img src=\"/imagens/consultar.gif\" border=0 title=\"Termo Ativo\" onClick=\"visualizarTermo( ''?modulo=principal/bensmovimentacao/visualizarTermo&acao=A&mvbid='|| movben.mvbid || ''' );\" style=\"cursor:pointer;\">'
           END 
           || '</center>' as comando,
           
           ter.ternum AS ternum, 
           -- ter.usucpf as Cpf, 
           usu.usunome AS Responsavel, 
           
           -- Unidade
           uniorg.uorno AS Unidade,
        
           -- Tipo
           'Movimentação' || ' (' ||ter.tertipo|| ')' AS Tipo,
           
           -- Data
           to_char(teremissaodt,'DD/MM/YYYY') AS data,
           -- movben.nu_matricula_siape,
        
           CASE WHEN EXISTS(SELECT pro.ternum
                            FROM sap.termo pro
                            WHERE pro.ternumanterior = ter.ternum)
              THEN (SELECT 'Substituído pelo termo ' || pro.ternum AS texto
                    FROM sap.termo pro
                    WHERE pro.ternumanterior = ter.ternum)
              ELSE 'Válido'
           END AS estado
            
        FROM 
           sap.termo ter
           INNER JOIN seguranca.usuario usu ON 
              usu.usucpf=ter.usucpf
           INNER JOIN sap.movimentacaobens movben ON 
              movben.ternum=ter.ternum
           LEFT JOIN siorg.uorgendereco orgend ON 
              orgend.uendid=movben.uendid
           LEFT JOIN siorg.unidadeorganizacional uniorg ON 
              uniorg.uorco_uorg_lotacao_servidor = orgend.uorco_uorg_lotacao_servidor
        WHERE 
        -- Tipo do Termo: MOVIMENTACAO    
        ter.tertipo='MV'";

		// FILTROS MOVIMENTACAO
		if(!empty($ternum)){
			$sqlBase .= "AND( ter.ternum='".$ternum."') ";
		}
		if(!empty($tertipo)){
			$sqlBase .= "AND(ter.tertipo='".$tertipo."') ";
		}
		if(!empty($teremissaodt_ini)){
			$sqlBase .= "AND(ter.teremissaodt>='".formata_data_sql($teremissaodt_ini)."') ";
		}
		if(!empty($teremissaodt_fim)){
			$sqlBase .= "AND(ter.teremissaodt<='".formata_data_sql($teremissaodt_fim)."') ";
		}
		if(!empty($nu_matricula_siape)){
			//$sql .= "AND nu_matricula_siape LIKE '%".$nu_matricula_siape."%' ";
		}
		// Resposansavel
		if(!empty($no_servidor)){
			$sqlBase .= "AND (ter.usucpf = (SELECT * FROM seguranca.usuario WHERE usunome LIKE '%".$no_servidor."%'))";
		}
		if(!empty($unidade)){
			$sqlBase .= "AND(uniorg.UORNO LIKE'%".$unidade."%') ";
		}
		
		if(!empty($matid)){

			$sqlBase .= "
			AND movben.mvbid IN (
            SELECT bmr.mvbid
            FROM sap.bensmovimentacaorgp bmr
               INNER JOIN sap.rgp rgp ON
                   rgp.rgpid = bmr.rgpid
               INNER JOIN sap.bensmaterial bmt ON
                   bmt.bmtid = rgp.bmtid
               INNER JOIN sap.material mat ON
                   mat.matid = bmt.matid
            WHERE MAT.MATID=".$matid .")";
        
		}
		
		/*
		if(!empty($nu_matricula_siape)){
			$sql .= "AND nu_matricula_siape LIKE '%".$nu_matricula_siape."%' ";
		}
		*/
		//ver($_POST);
        
        /*
        -- Número do Termo:   
        AND (ter.ternum='PARAMETRO') 
        -- Data do Termo:
        AND (teremissaodt>='PARAMETRO') 
        AND(teremissaodt<='PARAMETRO') 
        -- Responsável pelo Termo (SIAPE): #### CPF ####     
        AND (ter.usucpf = 'PARAMETRO')   
        -- Número do Processo: MOVIMENTACAO NAO TEM
        -- Número do Empenho: MOVIMENTACAO NAO TEM
        -- Material:     
        AND movben.mvbid IN (
            SELECT bmr.mvbid
            FROM sap.bensmovimentacaorgp bmr
               INNER JOIN sap.rgp rgp ON
                   rgp.rgpid = bmr.rgpid
               INNER JOIN sap.bensmaterial bmt ON
                   bmt.bmtid = rgp.bmtid
               INNER JOIN sap.material mat ON
                   mat.matid = bmt.matid
            WHERE MAT.MATDSC LIKE '%PARAMETRO%')
        
        -- Unidade Organizacional:
         AND (uniorg.UORNO LIKE '%PARAMETRO%')
        */
		$sqlBase .= "
        UNION 
        -- ENTRADA
        SELECT DISTINCT
           '<center>' ||
           CASE WHEN                    
            ter.terstatus=0
           THEN 
            '<img src=\"/imagens/consultar_desabilitado.gif\" border=0 title=\"Termo Inativo\">' 
           ELSE 
              '<img src=\"/imagens/consultar.gif\" border=0 title=\"Termo Ativo\" onClick=\"visualizarTermo(  ''?modulo=principal/bens/visualizarTermo&acao=A&benid=' || bem.benid || '''  );\"  style=\"cursor:pointer;\">'
           END 
           || '</center>' AS comando,
        
           ter.ternum AS ternum, 
           -- ter.usucpf as Cpf, 
           usu.usunome AS Responsavel, 
           
           -- Unidade
           uniorg.uorno AS Unidade,
        
           -- Tipo
           'Entrada' || ' (' ||ter.tertipo|| ')' AS Tipo,
        
           -- Data
           to_char(teremissaodt,'DD/MM/YYYY') AS data,
           -- movben.nu_matricula_siape,
        
           CASE WHEN EXISTS(SELECT pro.ternum
                            FROM sap.termo pro
                            WHERE pro.ternumanterior = ter.ternum)
              THEN (SELECT 'Substituído pelo termo ' || pro.ternum AS texto
                    FROM sap.termo pro
                    WHERE pro.ternumanterior = ter.ternum)
              ELSE 'Válido'
           END AS estado
        
        FROM 
           sap.termo ter
           INNER JOIN seguranca.usuario usu ON 
              usu.usucpf=ter.usucpf
           INNER JOIN sap.bens bem ON 
              bem.ternum=ter.ternum
           LEFT JOIN siorg.uorgendereco orgend ON 
              orgend.uendid=bem.uendid
           LEFT JOIN siorg.unidadeorganizacional uniorg ON 
              uniorg.uorco_uorg_lotacao_servidor = orgend.uorco_uorg_lotacao_servidor
        WHERE 
        -- Tipo do Termo: ENTRADA
        ter.tertipo='EN'
        ";
        
		// FILTROS ENTRADA
		if(!empty($ternum)){
			$sqlBase .= "AND( ter.ternum='".$ternum."') ";
		}
		if(!empty($tertipo)){
			$sqlBase .= "AND(ter.tertipo='".$tertipo."') ";
		}
		if(!empty($teremissaodt_ini)){
			$sqlBase .= "AND(ter.teremissaodt>='".formata_data_sql($teremissaodt_ini)."') ";
		}
		if(!empty($teremissaodt_fim)){
			$sqlBase .= "AND(ter.teremissaodt<='".formata_data_sql($teremissaodt_fim)."') ";
		}
		if(!empty($rgpnum)){
			$sqlBase .= "AND (bem.bennumproc ='".$rgpnum."') ";
		}
		if(!empty($empnumero)){
			$sqlBase .="
				AND bem.empid IN (
            	SELECT emp.empid 
            	FROM sap.empenho emp
            	WHERE emp.empnumero = '".$empnumero."')
            	";
		}

		if(!empty($matdsc)){
			$sqlBase .= "
                AND bem.benid IN (
                SELECT bmt.benid
                FROM sap.bensmaterial bmt 
                   INNER JOIN sap.material mat ON mat.matid = bmt.matid
                --WHERE MAT.MATID=".$matid.")
                WHERE MAT.MATDSC LIKE '%".$matdsc."%')
            	";
		}

		if(!empty($unidade)){
			$sqlBase .= "AND(uniorg.UORNO LIKE'%".$unidade."%') ";
		}

        
        /*
        -- Número do Termo:   
        AND (ter.ternum='PARAMETRO') 
        -- Data do Termo:
        AND (teremissaodt>='PARAMETRO') 
        AND(teremissaodt<='PARAMETRO') 
        -- Responsável pelo Termo (SIAPE): #### CPF ####     
        AND (ter.usucpf = 'PARAMETRO')   
        -- Número do Processo: 
        AND (bem.bennumproc = 'PARAMETRO')   
        -- Número do Empenho: 
        AND bem.empid IN (
            SELECT emp.empid 
            FROM sap.empenho emp
            WHERE emp.empnumero = 'PARAMETRO')
        -- Material:     
        AND bem.benid IN (
            SELECT bmt.benid
            FROM sap.bensmaterial bmt 
               INNER JOIN sap.material mat ON mat.matid = bmt.matid
            WHERE MAT.MATDSC LIKE '%PARAMETRO%')
        -- Unidade Organizacional:
        AND (uniorg.UORNO LIKE '%PARAMETRO%')
        */
        
        $sqlBase .="
        UNION
        -- BAIXA
         SELECT DISTINCT
           '<center>' ||
           CASE WHEN                    
            ter.terstatus=0
           THEN 
            '<img src=\"/imagens/consultar_desabilitado.gif\" border=0 title=\"Termo Inativo\">' 
           ELSE 
              '<img src=\"/imagens/consultar.gif\" border=0 title=\"Termo Ativo\" onClick=\"visualizarTermo(  ''?modulo=principal/baixadebens/visualizarTermo&acao=A&bebid=' || beb.bebid || '''  );\">'
              
           END || '</center>' AS comando,
        
           ter.ternum AS ternum, 
           -- ter.usucpf as Cpf, 
           usu.usunome AS Responsavel, 
           
           -- Unidade
           -- VER NOTA NOS JOINS ABAIXO
           '' AS Unidade, --uniorg.uorno 
        
           -- Tipo
           'Desfazimento' || ' (' ||ter.tertipo|| ')' AS Tipo,
        
           -- Data
           to_char(teremissaodt,'DD/MM/YYYY') AS data,
           -- movben.nu_matricula_siape
        
           CASE WHEN EXISTS(SELECT pro.ternum
                            FROM sap.termo pro
                            WHERE pro.ternumanterior = ter.ternum)
              THEN (SELECT 'Substituído pelo termo ' || pro.ternum AS texto
                    FROM sap.termo pro
                    WHERE pro.ternumanterior = ter.ternum)
              ELSE 'Válido'
           END AS estado
        
        FROM 
           sap.termo ter
           INNER JOIN seguranca.usuario usu ON 
              usu.usucpf=ter.usucpf
           INNER JOIN sap.bensbaixa beb ON
               beb.ternum = ter.ternum
        /* NAO EXISTE LIGACAO ENTRE A BAIXA E A TABELA DE ENDERECOS DE UNIDADES ORGANIZACIONAIS
           VERIFICAR COM ANALISTA	
           LEFT JOIN siorg.uorgendereco orgend ON 
              orgend.uendid=beb.uendid
           LEFT JOIN siorg.unidadeorganizacional uniorg ON 
              uniorg.uorco_uorg_lotacao_servidor = orgend.uorco_uorg_lotacao_servidor
        */      
        WHERE 
        -- Tipo do Termo: BAIXA
        ter.tertipo='BA'";
        
		// FILTROS BAIXA
		if(!empty($ternum)){
			$sqlBase .= "AND( ter.ternum='".$ternum."') ";
		}
		if(!empty($tertipo)){
			$sqlBase .= "AND(ter.tertipo='".$tertipo."') ";
		}
		if(!empty($teremissaodt_ini)){
			$sqlBase .= "AND(ter.teremissaodt>='".formata_data_sql($teremissaodt_ini)."') ";
		}
		if(!empty($teremissaodt_fim)){
			$sqlBase .= "AND(ter.teremissaodt<='".formata_data_sql($teremissaodt_fim)."') ";
		}
		if(!empty($rgpnum)){
			$sqlBase .= "AND (beb.bebnumprocesso ='".$rgpnum."') ";
		}
		
		// Resposansavel
		if(!empty($no_servidor)){
			$sqlBase .= "AND (ter.usucpf = (SELECT * FROM seguranca.usuario WHERE usunome LIKE '%".$no_servidor."%'))";
		}

		if(!empty($matdsc)){
			$sqlBase .= "
                AND (beb.bebid IN (
            SELECT bbr.bebid
            FROM sap.bensbaixargp bbr
               INNER JOIN sap.rgp rgp ON
                   rgp.rgpid = bbr.rgpid
               INNER JOIN sap.bensmaterial bmt ON
                   bmt.bmtid = rgp.bmtid
               INNER JOIN sap.material mat ON
                   mat.matid = bmt.matid
            WHERE MAT.MATDSC LIKE '%".$matdsc."%')
            	";
		}

		if(!empty($unidade)){
			//$sqlBase .= "AND (uniorg.UORNO LIKE'%".$unidade."%') ";
		}
        
        /*
        -- Número do Termo:   
        AND (ter.ternum='PARAMETRO') 
        -- Data do Termo:
        AND (teremissaodt>='PARAMETRO') 
        AND(teremissaodt<='PARAMETRO') 
        -- Responsável pelo Termo (SIAPE): #### CPF ####     
        AND (ter.usucpf = 'PARAMETRO')   
        -- Número do Processo: 
        AND (beb.bebnumprocesso = 'PARAMETRO')   
        -- Número do Empenho: BAIXA NAO TEM
        
        -- Material:     
        AND (beb.bebid IN (
            SELECT bbr.bebid
            FROM sap.bensbaixargp bbr
               INNER JOIN sap.rgp rgp ON
                   rgp.rgpid = bbr.rgpid
               INNER JOIN sap.bensmaterial bmt ON
                   bmt.bmtid = rgp.bmtid
               INNER JOIN sap.material mat ON
                   mat.matid = bmt.matid
            WHERE MAT.MATDSC LIKE '%PARAMETRO%'))
        -- Unidade Organizacional:
        -- VER NOTA NOS JOINS ACIMA
        -- AND (uniorg.UORNO LIKE '%PARAMETRO%')
        */
        
        $sqlBase .= "
        ) x
        ORDER BY x.ternum		
		";

				/*
				if(!empty($ternum)){
					$sql .= "AND( ter.ternum='".$ternum."') ";
				}
				if(!empty($tertipo)){
					$sql .= "AND(ter.tertipo='".$tertipo."') ";
				}
				if(!empty($teremissaodt_ini)){
					$sql .= "AND(ter.teremissaodt>='".formata_data_sql($teremissaodt_ini)."') ";
				}
				if(!empty($teremissaodt_fim)){
					$sql .= "AND(ter.teremissaodt<='".formata_data_sql($teremissaodt_fim)."') ";
				}
				
				if(!empty($nu_matricula_siape)){
					$sql .= "AND nu_matricula_siape LIKE '%".$nu_matricula_siape."%' ";
				}
				
				
				if(!empty($bennumproc)){
					$bennumproc = str_replace('.','',$bennumproc);
					$bennumproc = str_replace('-','',$bennumproc);
					$sql .= "AND(bem.bennumproc='".$bennumproc."') ";
				}
				if(!empty($bendtproc)){
					$sql .= "AND(bem.bendtproc='".formata_data_sql($bendtproc)."') ";
				}
				if(!empty($bendtentrada)){
					$sql .= "AND(bem.bendtentrada='".formata_data_sql($bendtentrada)."') ";
				}
				if(!empty($bebdata)){
					$sql .= "AND(bembaixa.bebdata='".formata_data_sql($bebdata)."') ";
				}
				if(!empty($rgpnum)){
					$sql .= "AND(rgp.rgpnum='".$rgpnum."') ";
				}
				if(!empty($rgpnumserie)){
					$sql .= "AND(rgp.rgpnumserie='".$rgpnumserie."') ";
				}
				if(!empty($sbmid)){
					$sql .= "AND(rgp.sbmid='".$sbmid."') ";
				}
				if(!empty($ecoid)){
					$sql .= "AND(estmotivos.ecoid='".$ecoid."') ";
				}
				if(!empty($mecid)){
					$sql .= "AND(estmotivos.mecid='".$mecid."') ";
				}
				if(!empty($matid)){
					$sql .= "AND(bemmaterial.matid='".$matid."') ";
				}
				if(!empty($nu_matricula_siape)){
					$sql .= "AND(bem.nu_matricula_siape='".$nu_matricula_siape."') ";
				}
				
				if(!empty($uendid)){
					$sql .= "AND(bem.uendid='".$uendid."') ";
				}
				else if(!empty($uorco_uorg_lotacao_servidor)){
					$sql .= "AND(uorgend.uorco_uorg_lotacao_servidor='".$uorco_uorg_lotacao_servidor."') ";
				}
				
				*/
				

				
				//monta array de dados para montar a grid de resultados
				//parecerá apenas uma vez cada rgp
				$dados = array();
				$rgpsJaForam = array();
				//$result = $this->carregar( $sqlBase . $sql);
				$result = $this->carregar( $sqlBase);
				
				
				if(is_array($result) && count($result) >= 1){
					foreach($result as $key => $value){
						if(!in_array($value['rgpnum'],$rgpsJaForam)){
							$rgpsJaForam[] = $value['rgpnum'];
							//trata a data
							$value['teremissaodt'] = formata_data($value['teremissaodt']);
							
							foreach($value as $key2 => $value2){
								$value[$key2] = '<center>'.$value2.'</center>';
							}
							
							$dados[] = $value;	
						}
					}
				}
				
	
				//$result = $this->carregar($sqlBase . $sql);
				/*
				 * SIM o codigo e execultado duas vezes. Nao apagar.
				 * Nao tenho ideia do porque
				 * */
				$result = $this->carregar($sqlBase);

				if(is_array($result) && count($result) >= 1){
					foreach($result as $key => $value){
						if(!in_array($value['ternum'],$rgpsJaForam)){
							$rgpsJaForam[] = $value['ternum'];
							//trata a data
							$value['teremissaodt'] = formata_data($value['teremissaodt']);
							//retira a data de movimentação usada apenas para ordenação
							array_pop($value);
							
							foreach($value as $key2 => $value2){
								$value[$key2] = '<center>'.$value2.'</center>';
							}
							
							$dados[] = $value;
						}
					}
				}
				

		/*
 		* P12. O sistema exibirá a lista de acordo com os critérios informados, a lista é composta das seguintes informações: 
 		* Comando, Nº do Termo, Responsável, Unidade Tipo, Data e Estado [RNG06];
 		* */
		//$cabecalho = array("Comando","RGP","Material","Número de Série","Estado","Data","Situação");
		$cabecalho = array("Comando", "Nº do Termo","Responsável","Unidade", "Tipo","Data","Estado");
		
		$this->monta_lista_array($dados,$cabecalho,20,50,false,"center");
	}
	
	/**
     * Verifica o status do termo
     * @name verificaTermoStatus
     * @author Apoena Machado Cunha
     * @access public
     * @param int $ternum
     * @return boolean
     */
	public function verificaTermoStatus( $ternum ){
	    return true;
	}
	
}
