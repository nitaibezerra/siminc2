<?php
	
class GestaoDocumentos extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "gestaodocumentos.tarefa";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tarid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos = array(
            'tarid'                     => null, 
            'tartipo'                   => null, 
            '_tartarefa'                => null, 
            '_tarpai'                   => null, 
            '_tarordem'                 => null, 
            '_tarprofundidade'          => null, 
            'tartitulo'                 => null, 
            'tardsc'                    => null, 
            'tartema'                   => null, 
            'tmdid'                     => null, 
            'tarprioridade'             => null, 
            'unaidsetororigem'          => null, 
            'unaidsetorresponsavel'     => null, 
            'usucpfresponsavel'         => null, 
            'sitid'                     => null, 
            'tardatarecebimento'        => null, 
            'tardatainicio'             => null, 
            'tardataprazolinhadebase'   => null, 
            'tardataprazoatendimento'   => null, 
            'tardataconclusao'          => null,
            'tardepexterna'             => null,
            'tarnumsidoc'               => null,
            'tartiponumsidoc'           => null,
            'temid'                     => null,
            'taranobase'                => null,
            'nvcid'                     => null, 
            'tpeid'                     => null,
            'tarnumprocexterno'         => null,
            'tarsitprocexterno'         => null,
            'tarnumidentexterno'        => null,
            'tarsitarquivo'             => null,
            'tarporcentoexec'		=> null,
            'tarrecebimento'		=> null,
            'tarcomplexidade'		=> null
      );
									  
	public function depoisSalvar(){
		$this->_tartarefa = self::pegaTartarefaPorTarid($this->tarid);
		if(!$this->_tartarefa){
			$this->_tartarefa = $this->tarid;
			$this->salvar(false,false);
		}
	}
	
	public function antesExcluir( $id = null ){
		$boTemFilhos = $this->pegaUm("select tarid from gestaodocumentos.tarefa where _tarpai = {$id} ");
		$objDemandas = $this->pegaUm("select tarid from gestaodocumentos.objetodemanda where tarid = {$id} ");
		$objSolicita = $this->pegaUm("select tarid from gestaodocumentos.instituicaosolicitante where tarid = {$id} ");
		$objPalavra = $this->pegaUm("select tarid from gestaodocumentos.palavrachave where tarid = {$id} ");
		$objReitera = $this->pegaUm("select taridprincipal from gestaodocumentos.reiteracoes where taridprincipal = {$id} ");
        
		if($boTemFilhos){
			echo "<script>
					alert('Existem Atividades Vinculadas a esta Demanda / Atividade. Favor excluir Atividade vinculadas.');
					history.back(-1);
				  </script>";
			die;
		} elseif($objDemandas){
            echo "<script>
					alert('Existem Objeto da Demanda vinculadas a esta Demanda. Favor excluir Objeto da Demanda vinculadas.');
					history.back(-1);
				  </script>";
			die;
        }elseif($objSolicita){
            echo "<script>
					alert('Existem Solicitantes vinculados a esta Demanda. Favor excluir Solicitante vinculado.');
					history.back(-1);
				  </script>";
			die;
        }elseif($objPalavra){
            echo "<script>
					alert('Existem Expressões Chave vinculadas a esta Demanda. Favor excluir Expressões Chave vinculadas.');
					history.back(-1);
				  </script>";
			die;
        }elseif($objReitera){
            echo "<script>
					alert('Existem Reiterações vinculadas a esta Demanda. Favor excluir Reiterações vinculadas.');
					history.back(-1);
				  </script>";
			die;
        }else{
						
			# Excluir Acompanhamento Relacionados
			$sql = "delete from gestaodocumentos.acompanhamento where tarid = {$id} ";
			if ( !$this->executar($sql)){
				return false;
			}
			
			# Excluir Documentos
			$sql = "select arqid from gestaodocumentos.anexo where tarid = {$id} ";
			$arqid = $this->pegaUm($sql);
			
			if($arqid){
				$sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid = {$arqid}";
				if ( !$this->executar($sql)){
					return false;
				}
				$sql = "delete from gestaodocumentos.anexo where tarid = {$id} ";
				if ( !$this->executar($sql)){
					return false;
				}
			}

			# Excluir Restricoes
			$sql = "delete from gestaodocumentos.restricao where tarid = {$id} ";
			if ( !$this->executar($sql)){
				return false;
			}
			return true;
			//die;
		}
	}
	
	public function recuperaInstituicoesPorTarid($tarid){
		$instituicoes = "";
		$arDadosTemp = array();
		$arDados = $this->carregar("select iesid from gestaodocumentos.instituicaorelacionada where tarid = {$tarid} ");
		if($arDados){
			foreach($arDados as $dados){
				$arDadosTemp[] = $dados['iesid'];
			}
			$entid = implode(',',$arDadosTemp);
  			$instituicoes = $this->carregar("SELECT e.iesid as codigo, e.iesnome as descricao
						  					FROM ies.ies e
					  						where e.iesid in (".$entid.")
											limit 10 ");
		}
		return $instituicoes;		
	}
	
	public function recuperaSolicitantesPorTarid($tarid){
		$arDados = $this->carregar("select solnome from gestaodocumentos.solicitante where tarid = {$tarid} ");
		$arDados = ($arDados) ? $arDados : array();
		return $arDados;
	}
	
	public function recuperaNomeSetorRepon($unaid){
		return $this->pegaUm("select unadescricao from gestaodocumentos.unidade where unaid = {$unaid} ");
	}
	
	public function recuperaNomePessoaRepon($cpf){
		if(!$cpf){
			return "Usuário Indefinido";
		}
		return $this->pegaUm("select
								case when usunome is null then 'Usuário Indefinido' 
								else usunome
								end as nome
							from seguranca.usuario where usucpf = '{$cpf}' ");
	}
	
	public function recuperaNomeSituacao($sitid){
		return $this->pegaUm("select sitdsc from gestaodocumentos.situacaotarefa where sitid = '{$sitid}' ");
	}
	
	public function salvarAcompanhamento($post, $boSomenteMsg = false){
		extract($post);
		$acodscTemp = "";
		if(!$boSomenteMsg){
			if($this->unaidsetorresponsavel != $unaidsetorresponsavelAnterior){
				$acodscTemp .= "Setor Responsável: de ( {$this->recuperaNomeSetorRepon($unaidsetorresponsavelAnterior)} ) para ( {$this->recuperaNomeSetorRepon($this->unaidsetorresponsavel)} ) <br>";
			}
			if($this->usucpfresponsavel != $usucpfresponsavelAnterior){
				$acodscTemp .= "Pessoa Responsável: de ( {$this->recuperaNomePessoaRepon($usucpfresponsavelAnterior)} ) para ( {$this->recuperaNomePessoaRepon($this->usucpfresponsavel)} ) <br>";
			}
			$obData = new Data();
			$tardataprazoatendimento = $obData->formataData($this->tardataprazoatendimento,"dd/mm/YYYY");
			if($tardataprazoatendimento != $tardataprazoatendimentoAnterior){
				$acodscTemp .= "Prazo de Atendimento: de ( {$tardataprazoatendimentoAnterior} ) para ( {$tardataprazoatendimento} ) <br>";
			}
			if($this->sitid != $sitidAnterior){
				$acodscTemp .= "Situação: de ( {$this->recuperaNomeSituacao($sitidAnterior)} ) para ( {$this->recuperaNomeSituacao($this->sitid)} ) <br>";
			}
			if($acodscTemp){
				$acodscTemp = "Dados Alterados: ".$acodscTemp;
			}
		}
		$acodscTemp .= $acodsc;
		$sql = "INSERT INTO gestaodocumentos.acompanhamento (
					usucpf,
					tarid,
					acodsc,
					acostatus,
					acodata
				) VALUES (
					'".$_SESSION['usucpf']."', 
					'".$this->tarid."',
					'".$acodscTemp."',
					'A',
					now()
				);";
		$this->executar($sql);
	}
	
	public function salvarAcompanhamentoPelaArvore($post, $coluna, $boGravaMensagem = true){
		extract($post);
		$acodscTemp = "";
		header('content-type: text/html; charset=ISO-8859-1');
		
		switch($coluna){
			case 'respon':
				if($usucpfresponsavel != $usucpfresponsavelAnterior){
					$acodscTemp .= "Pessoa Responsável: de ( {$this->recuperaNomePessoaRepon($usucpfresponsavelAnterior)} ) para ( {$this->recuperaNomePessoaRepon($usucpfresponsavel)} ) <br>";
				}
			break;
			case 'situacao':
                if($sitid != $sitidAnterior){
					$acodscTemp .= "Situação: de ( {$this->recuperaNomeSituacao($sitidAnterior)} ) para ( {$this->recuperaNomeSituacao($sitid)} ) <br>";
				}
			break;
			case 'prazo':
				if($tardataprazoatendimento != $tardataprazoatendimentoAnterior){
					$acodscTemp .= "Prazo de Atendimento: de ( {$tardataprazoatendimentoAnterior} ) para ( {$tardataprazoatendimento} ) <br>";
				}
			break;
				
		}
		if($acodscTemp){
			$acodscTemp = "<b>Dados Alterados</b><br />".$acodscTemp;
			$quebra = "<br />";
		}
		
		if($boGravaMensagem){
			//$acodscTemp .= "$quebra<b>Mensagem</b><br />".iconv( "UTF-8", "ISO-8859-1", $acodsc);
			$acodscTemp .= "$quebra<b>Mensagem</b><br />".$acodsc;
		}
		
		$sql = "INSERT INTO gestaodocumentos.acompanhamento (
					usucpf,
					tarid,
					acodsc,
					acostatus,
					acodata
				) VALUES (
					'".$_SESSION['usucpf']."', 
					'".$this->tarid."',
					'".addslashes($acodscTemp)."',
					'A',
					now()
				);";
		$this->executar($sql);
	}
	
	public function pegaTartarefaPorTarid($tarid){
		if($tarid)
			return $this->pegaUm("select _tartarefa from gestaodocumentos.tarefa where tarid = $tarid ");
	}
	
	public function boAtividade(){
		return $this->pegaUm("select _tarpai from gestaodocumentos.tarefa where tarid = $this->tarid ");
	}
	
	public function recuperaAcompanhamentoTarid($tarid){
            $sql = "";
            if($tarid){
                $sql = "
                    SELECT  to_char(a.acodata, 'DD/MM/YYYY HH24:MI:SS') AS data,
                            u.usunome,
                            a.acodsc as acodsc
                    FROM gestaodocumentos.acompanhamento AS a 
                    LEFT JOIN seguranca.usuario AS u ON u.usucpf = a.usucpf

                    WHERE a.tarid = '{$tarid}' AND a.acostatus = 'A' 

                    ORDER BY a.acoid DESC
                ";
            }
            return $this->carregar($sql);
	}
	
	public function recuperaEmailPorCpf($cpf){
		if($cpf)
			return $this->pegaUm("select usuemail from seguranca.usuario where usucpf = '$cpf' ");
	}
	
	public function pegaAtividade($tarid){
		$sql = "select 
					_tarordem, _tartarefa 
				from gestaodocumentos.tarefa where tarid = $tarid";
		$dados = $this->pegalinha($sql);
		$max_tarordem = str_split($dados['_tarordem'], 4);
		$nratividade = ""; 
				
		if($max_tarordem){
			for($i=1;$i<count($max_tarordem);$i++){
				$nratividade .= intval($max_tarordem[$i]).".";
			}
			$nratividade = substr($nratividade,0,-1);
			$nratividade = ($nratividade=='' ? '': '.'.$nratividade);
			$max_tarordem = $dados['_tartarefa'].$nratividade;
		}
		else
			$max_tarordem = "S/Nº";
		
		return $max_tarordem;
	}

	public function pegaEstadoDocumento($tarid){
		$sql = "SELECT 	 	e.esddsc	 
				FROM	 	gestaodocumentos.tarefa t 
				INNER JOIN	workflow.documento d ON d.docid = t.docid
				INNER JOIN	workflow.estadodocumento e ON e.esdid = d.esdid
				WHERE 	 	t.tarid = {$tarid}";
		
		$dados = $this->pegaUm($sql);
		return $dados;
	}
}