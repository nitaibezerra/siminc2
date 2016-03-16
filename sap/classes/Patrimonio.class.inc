<?php
/**
 * Modelo de Patrimônio
 * @author Alysson Rafael
 */
class Patrimonio extends Modelo{
	
	/**
     * Lista os dados do patrimônio
     * @name listaPatrimonios
     * @author Alysson Rafael
     * @access public
     * @param array $post - Dados do formulário
     * @return void
     */
	public function listaPatrimonios($post = array()){
		extract($post);
						
		$sql = "SELECT DISTINCT
				'<center><a style=\"cursor:pointer;\" onclick=\"javascript: window.location.href = \'sap.php?modulo=principal/patrimonio/dadosPatrimonio&acao=A&rgpid='||rgp.rgpid||'\'\");\">
                 <img src=\"/imagens/alterar.gif \" border=0 title=\"Detalhar\">
                 </a>
                </center>' as acao, 
				rgp.rgpnum||' ' AS rgpnum,	
				material.matdsc,
				rgp.rgpnumserie||' ' AS rgpnumserie,
				estconservacao.ecodsc,
				bem.bendtentrada,
				situacao.sbmdsc
			   
				FROM sap.rgp rgp
				JOIN sap.bensmaterial bemmaterial ON rgp.bmtid=bemmaterial.bmtid
				JOIN sap.bens bem ON bemmaterial.benid=bem.benid
				JOIN sap.material material ON bemmaterial.matid=material.matid
				LEFT JOIN sap.bensbaixargp bembaixargp ON rgp.rgpid=bembaixargp.rgpid
				LEFT JOIN sap.bensbaixa bembaixa ON bembaixargp.bebid=bembaixa.bebid
				LEFT JOIN siorg.uorgendereco uorgend ON bem.uendid=uorgend.uendid
				LEFT JOIN siorg.unidadeorganizacional unidade ON uorgend.uorco_uorg_lotacao_servidor=unidade.uorco_uorg_lotacao_servidor
				LEFT JOIN siorg.enderecoandarsala endandarsala ON uorgend.easid=endandarsala.easid
				LEFT JOIN siorg.enderecoandar endandar ON endandarsala.enaid=endandar.enaid
				LEFT JOIN siorg.endereco endereco ON endandar.endid=endereco.endid
				LEFT JOIN sap.bensmovimentacaorgp movimentargp ON rgp.rgpid=movimentargp.rgpid
				LEFT JOIN sap.movimentacaobens movimenta ON movimentargp.mvbid=movimenta.mvbid
				JOIN sap.estadomotivos estmotivos ON rgp.esmid=estmotivos.esmid
				JOIN sap.estadoconservacao estconservacao ON estmotivos.ecoid=estconservacao.ecoid
				JOIN sap.situacaobem situacao ON rgp.sbmid=situacao.sbmid

				WHERE(
				(NOT EXISTS(SELECT movimentargp.mvbid FROM sap.bensmovimentacaorgp movimentargp WHERE rgp.rgpid=movimentargp.rgpid))";
				
				if(!empty($tesid1)){
					$sql .= "AND(bem.tesid='".$tesid1."') ";
				}
				if(!empty($tesid)){
					$sql .= "AND(bembaixa.tesid='".$tesid."') ";
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
				
				
				$sql .= ") ORDER BY rgpnum ";
				
				//monta array de dados para montar a grid de resultados
				//parecerá apenas uma vez cada rgp
				$dados = array();
				$rgpsJaForam = array();
				$result = $this->carregar($sql);
				
				if(is_array($result) && count($result) >= 1){
					foreach($result as $key => $value){
						if(!in_array($value['rgpnum'],$rgpsJaForam)){
							$rgpsJaForam[] = $value['rgpnum'];
							//trata a data
							$value['bendtentrada'] = formata_data($value['bendtentrada']);
							
							foreach($value as $key2 => $value2){
								$value[$key2] = '<center>'.$value2.'</center>';
							}
							
							$dados[] = $value;	
						}
					}
				}
				
				
				
		$sql = "SELECT DISTINCT
				'<center><a style=\"cursor:pointer;\" onclick=\"javascript: window.location.href = \'sap.php?modulo=principal/patrimonio/dadosPatrimonio&acao=A&rgpid='||rgp.rgpid||'\'\");\">
                 <img src=\"/imagens/alterar.gif \" border=0 title=\"Detalhar\">
                 </a>
                </center>' as acao, 
				rgp.rgpnum||' ' AS rgpnum,	
				material.matdsc,
				rgp.rgpnumserie||' ' AS rgpnumserie,
				estconservacao.ecodsc,
				bem.bendtentrada,
				situacao.sbmdsc,
				movimenta.mvbdata 
				
				FROM sap.rgp rgp 
				JOIN sap.bensmaterial bemmaterial ON rgp.bmtid=bemmaterial.bmtid
				JOIN sap.bens bem ON bemmaterial.benid=bem.benid
				JOIN sap.material material ON bemmaterial.matid=material.matid
				LEFT JOIN sap.bensbaixargp bembaixargp ON rgp.rgpid=bembaixargp.rgpid
				LEFT JOIN sap.bensbaixa bembaixa ON bembaixargp.bebid=bembaixa.bebid
				LEFT JOIN siorg.uorgendereco uorgend ON bem.uendid=uorgend.uendid
				LEFT JOIN siorg.unidadeorganizacional unidade ON uorgend.uorco_uorg_lotacao_servidor=unidade.uorco_uorg_lotacao_servidor
				LEFT JOIN siorg.enderecoandarsala endandarsala ON uorgend.easid=endandarsala.easid
				LEFT JOIN siorg.enderecoandar endandar ON endandarsala.enaid=endandar.enaid
				LEFT JOIN siorg.endereco endereco ON endandar.endid=endereco.endid
				JOIN sap.bensmovimentacaorgp movimentargp ON rgp.rgpid=movimentargp.rgpid
				JOIN sap.movimentacaobens movimenta ON movimentargp.mvbid=movimenta.mvbid
				JOIN sap.estadomotivos estmotivos ON rgp.esmid=estmotivos.esmid
				JOIN sap.estadoconservacao estconservacao ON estmotivos.ecoid=estconservacao.ecoid
				JOIN sap.situacaobem situacao ON rgp.sbmid=situacao.sbmid
				
				WHERE(1=1 ";

				if(!empty($nu_matricula_siape)){
					$sql .= "AND((SELECT movimenta1.nu_matricula_siape 
					              FROM sap.movimentacaobens movimenta1 
					              JOIN sap.bensmovimentacaorgp movimentargp1 ON movimenta1.mvbid=movimentargp1.mvbid 
					              WHERE rgp.rgpid=movimentargp1.rgpid ORDER BY movimenta1.mvbdata DESC LIMIT 1)='".$nu_matricula_siape."') ";
				}
				if(!empty($tesid1)){
					$sql .= "AND(bem.tesid='".$tesid1."') ";
				}
				if(!empty($tesid)){
					$sql .= "AND(bembaixa.tesid='".$tesid."') ";
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
				
				if(!empty($uendid)){
					$sql .= "AND((SELECT movimenta1.uendid
					 			  FROM sap.movimentacaobens movimenta1
					 			  JOIN sap.bensmovimentacaorgp movimentargp1 ON movimenta1.mvbid=movimentargp1.mvbid
					 			  WHERE rgp.rgpid=movimentargp1.rgpid ORDER BY movimenta1.mvbdata DESC LIMIT 1)='".$uendid."')";
				}
				else if(!empty($uorco_uorg_lotacao_servidor)){
					$sql .= "AND((SELECT uorgend.uorco_uorg_lotacao_servidor
					         FROM siorg.uorgendereco uorgend
					         JOIN sap.movimentacaobens movimenta1 ON movimenta1.uendid=uorgend.uendid
					         JOIN sap.bensmovimentacaorgp movimentargp1 ON movimenta1.mvbid=movimentargp1.mvbid
					         WHERE rgp.rgpid=movimentargp1.rgpid ORDER BY movimenta1.mvbdata DESC LIMIT 1)='".$uorco_uorg_lotacao_servidor."')";
				}
				
				
				$sql .= ") ORDER BY rgpnum ASC,mvbdata DESC ";
               
				
				
				$result = $this->carregar($sql);

				if(is_array($result) && count($result) >= 1){
					foreach($result as $key => $value){
						if(!in_array($value['rgpnum'],$rgpsJaForam)){
							$rgpsJaForam[] = $value['rgpnum'];
							//trata a data
							$value['bendtentrada'] = formata_data($value['bendtentrada']);
							//retira a data de movimentação usada apenas para ordenação
							array_pop($value);
							
							foreach($value as $key2 => $value2){
								$value[$key2] = '<center>'.$value2.'</center>';
							}
							
							$dados[] = $value;
						}
					}
				}
				
				
				
		$cabecalho = array("Comando","RGP","Material","Número de Série","Estado","Data","Situação");
		$this->monta_lista_array($dados,$cabecalho,20,50,false,"center");
	}
	
	/**
     * Carrega os dados do patrimônio pelo id do rgp
     * @name carregaRGPPorRGPId
     * @author Alysson Rafael
     * @access public
     * @param int $rgpid - Id do RGP
     * @return array
     */
	public function carregaRGPPorRGPId($rgpid){
		
		$dataHojeSql = formata_data_sql(date("d/m/Y"));
		
		$sql = "SELECT
				    rgp.rgpnum,
				    mat.matdsc,
				    ben.bennumproc,
				    ben.bendtentrada,
				    bb.bebnumprocesso,
				    bb.bebdata,
				    rgp.rgpnumserie,
				    situacao.sbmdsc,
				    estconservacao.ecodsc,
				    motestconserv.mecdsc,
                                    x.vliquido as valhistorico,
                                    x.vdepreciacaoacumulada as valdepreciado
				FROM
				    sap.rgp rgp
                                    INNER JOIN sap.fn_relatorio_depreciacao_mensal('{$dataHojeSql}') x
                                    ON rgp.rgpnum = x.rgpnum
				    JOIN sap.bensmaterial bm ON bm.bmtid=rgp.bmtid
				    JOIN sap.bens ben ON ben.benid=bm.benid
				    LEFT JOIN sap.bensbaixargp bbr ON bbr.rgpid=rgp.rgpid
				    LEFT JOIN sap.bensbaixa bb ON bb.bebid=bbr.bebid
				    JOIN sap.material mat ON mat.matid=bm.matid
				    JOIN sap.estadomotivos estmotivos ON rgp.esmid=estmotivos.esmid
				    JOIN sap.estadoconservacao estconservacao ON estmotivos.ecoid=estconservacao.ecoid
				    JOIN sap.motivoestadoconservacao motestconserv ON estmotivos.mecid=motestconserv.mecid
				    
				    
				    JOIN sap.situacaobem situacao ON rgp.sbmid=situacao.sbmid
				WHERE
					rgp.rgpid = '".$rgpid."' ";
		
		$arDados = $this->pegaLinha( $sql );
		if(is_array($arDados)){
			return $arDados;
		}
		else{
			return array();
		}
		
	}

	/**
     * Lista os ultimos responsaveis pelo patrimônio
     * @name listaUltimosResponsaveisPatrimonios
     * @author Alysson Rafael
     * @access public
     * @param int $rgpid - Id do RGP
     * @return void
     */
	public function listaUltimosResponsaveisPatrimonios($rgpid){
		$sql = array();
		if(!empty($rgpid)){
			
			$sql = "SELECT responsavel.nu_matricula_siape||' ' AS nu_matricula_siape,
			               responsavel.no_servidor,
			               unidade.uorno,
			               
			               CASE 
								WHEN CHAR_LENGTH(CAST(endereco.endnum AS text)) > 1 THEN
									endereco.endlog || ', ' || endereco.endnum || ', ' || endereco.endcom
								ELSE
									endereco.endlog || ', ' || endereco.endcom 
						   END as descricao
			               
			               
			               FROM siape.vwservidorativo responsavel
			               JOIN sap.movimentacaobens movimenta ON responsavel.nu_matricula_siape=movimenta.nu_matricula_siape 
			               JOIN sap.bensmovimentacaorgp movimentargp ON movimenta.mvbid=movimentargp.mvbid
			               JOIN sap.rgp rgp ON movimentargp.rgpid=rgp.rgpid
			               JOIN sap.bensmaterial bemmaterial ON rgp.bmtid=bemmaterial.bmtid
			               JOIN sap.bens bem ON bemmaterial.benid=bem.benid
			               JOIN siorg.uorgendereco uorgend ON bem.uendid=uorgend.uendid
			               JOIN siorg.unidadeorganizacional unidade ON uorgend.uorco_uorg_lotacao_servidor=unidade.uorco_uorg_lotacao_servidor
			               JOIN siorg.enderecoandarsala endandarsala ON uorgend.easid=endandarsala.easid
			               JOIN siorg.enderecoandar endandar ON endandarsala.enaid=endandar.enaid
			               JOIN siorg.endereco endereco ON endandar.endid=endereco.endid
			               WHERE rgp.rgpid='".$rgpid."' ORDER BY movimenta.mvbdata DESC ";
		}
		
		$cabecalho = array("SIAPE","Servidor","Unidade Organizacional","Localização");
		$this->monta_lista($sql,$cabecalho,20,50,false,"center",'S','','',array('center','center','center','center'));
	}
}
?>