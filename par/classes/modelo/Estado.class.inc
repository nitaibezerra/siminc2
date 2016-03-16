<?php
	
class Estado extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "territorios.estado";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "estuf" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'estuf' => null, 
									  	'muncodcapital' => null, 
									  	'regcod' => null, 
									  	'estcod' => null, 
									  	'estdescricao' => null, 
									  );

	public function lista($arWhere = array(), $arInner = array(), $tipo = '')
	{
		if( $tipo == 'doc' ){
			$cabecalho = array("Selecione","Sigla","Unidade da Federação", "Situação", "Restrição");
			$cache = ''; 
			$acoes = "case when iu.inutermoliberado = false then 
							'<center><input type=\"checkbox\" name=\"libera[]\" value=\"'||e.estuf||'\">&nbsp;
									<input type=\"hidden\" id=\"hid_select\" name=\"hid_select[]\" value=\"'||e.estuf||'\">
									<img src=../imagens/report.gif style=cursor:pointer; title=Documentos onclick=\"window.open(\'par.php?modulo=principal/listaDocumentosParaAssinar&acao=A&estuf='||e.estuf||'\',\'visualizaDocumento\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\"></center>'
						else
							'<center><input type=\"checkbox\" checked name=\"libera[]\" value=\"'||e.estuf||'\">
									<input type=\"hidden\" id=\"hid_select\" name=\"hid_select[]\" value=\"'||e.estuf||'\">
									<img src=../imagens/report.gif style=cursor:pointer; title=Documentos onclick=\"window.open(\'par.php?modulo=principal/listaDocumentosParaAssinar&acao=A&estuf='||e.estuf||'\',\'visualizaDocumento\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\">
							</center>'
						end";
		} else {
			$cabecalho = array("Ação","Sigla","Unidade da Federação", "Situação", "Restrição");
			
			
			$arrPerfil = pegaPerfilGeral();
			$arrPerfil = (is_array($arrPerfil)) ? $arrPerfil : array();
			
			if (count(array_intersect(unserialize(GRUPO_DE_PERFIL_ESTADUAL), $arrPerfil)) > 0 || count(array_intersect(unserialize(GRUPO_DE_PERFIL_MUNICIPAL), $arrPerfil)) > 0)
			{
				$acoes = "'<center><a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abreExecucaoOrcamento(\'estado\',\''|| e.estuf ||'\',\'\',\''|| CASE WHEN iu.inuid IS NULL THEN 0 ELSE iu.inuid END 	||'\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a></center>'";
			}
			else
			{
				$acoes = "'<center><a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abrePlanoTrabalho(\'estado\',\''|| e.estuf ||'\',\'\',\''|| CASE WHEN iu.inuid IS NULL THEN 0 ELSE iu.inuid END 	||'\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a></center>'";
			}
			$cache = '3600';
		}
                
                $sqlRestricoes = ",CASE WHEN (SELECT 
                                    CASE 
                                        WHEN count(res.resid) >  0 THEN TRUE
                                        ELSE FALSE
                                    END 
                                FROM par.restricaoentidade re
                                INNER JOIN par.restricaofnde res ON re.resid = res.resid
                                LEFT JOIN workflow.documento doc ON doc.docid = re.docid 
                                LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                                WHERE re.inuid = iu.inuid 
                                AND res.resstatus = 'A'
                                AND esd.esdid NOT IN (".ESDID_CONCLUIDO.")
                                ) THEN 
					'<center> <a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abreRestricao(\'estado\',\''|| e.estuf ||'\',\'\')\"><img src=\"/imagens/atencao.png\" border=0 title=\"Restrições de UF\"></a> </center>'
				ELSE
				 	'<center>-</center>'
				END
					as restricoes";                
                
		$sql = "SELECT DISTINCT
					$acoes as acao, 
		            e.estuf,
		            e.estdescricao, 
		            CASE WHEN ed.esddsc IS NULL THEN 'Diagnóstico' ELSE ed.esddsc END as sit
		            $sqlRestricoes
				FROM territorios.estado AS e   
					LEFT JOIN par.instrumentounidade iu ON iu.estuf = e.estuf
					LEFT JOIN workflow.documento d on d.docid = iu.docid AND d.tpdid = 44 
					LEFT JOIN workflow.estadodocumento ed on ed.esdid = d.esdid
					" . $arInner['join_dadosunidade'] . "
					" . ( is_array($arWhere) && count($arWhere) ? ' where ' . implode(' AND ', $arWhere) : '' ) ."
				ORDER BY e.estuf ASC
				";
		
		$tamanho   = array( '5%', '10%', '50%', '10%' );															
		return $this->monta_lista($sql,$cabecalho,50,5,'N','95%',$par2,'formlistaestado',$tamanho,'', '');
	}
	
	public function descricaoEstado($estuf)
	{
		return $this->pegaUm("SELECT estdescricao FROM territorios.estado where estuf = '$estuf' ");
	}
        
        public function carregarUf($booSQL){
            $sql = "SELECT 
                        estuf AS codigo,
                        estdescricao AS descricao 
                    FROM 
                        {$this->stNomeTabela}
                    ORDER BY 
                        estuf;";
            return ($booSQL) ? $sql : $this->carregar($sql);
        }
	
}