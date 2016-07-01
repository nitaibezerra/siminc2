<?php
	
class Tarefa extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "tarefa.tarefa";	

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
    protected $arAtributos     = array(
									  	'tarid' => null, 
									  	'tartipo' => null, 
									  	'_tartarefa' => null, 
									  	'_tarpai' => null, 
									  	'_tarordem' => null, 
									  	'_tarprofundidade' => null, 
									  	'tartitulo' => null, 
									  	'tardsc' => null, 
									  	'tartema' => null, 
									  	'tmdid' => null, 
									  	'tarprioridade' => null, 
									  	'unaidsetororigem' => null, 
									  	'unaidsetorresponsavel' => null, 
									  	'usucpfresponsavel' => null, 
									  	'sitid' => null, 
									  	'tardatarecebimento' => null, 
									  	'tardatainicio' => null, 
									  	'tardataprazolinhadebase' => null, 
									  	'tardataprazoatendimento' => null, 
									  	'tardataconclusao' => null,
									  	'tardepexterna' => null,
    									'tarnumsidoc' => null,
    									'tartiponumsidoc' => null
									  );
									  
	public function depoisSalvar(){
		$this->_tartarefa = self::pegaTartarefaPorTarid($this->tarid);
		if(!$this->_tartarefa){
			$this->_tartarefa = $this->tarid;
			$this->salvar(false,false);
		}
	}
	
	public function antesExcluir( $id ){
		$boTemFilhos = $this->pegaUm("select tarid from tarefa.tarefa where _tarpai = {$id} ");
		if($boTemFilhos){
			echo "<script>
					alert('Existem Atividades Vinculadas a esta Tarefa / Atividade. Favor excluir Atividade vinculadas.');
					history.back(-1);
				  </script>";
			die;
		} else {
			# Excluir Solicitantes Relacionados
			$sql = "delete from tarefa.solicitante where tarid = {$id} ";
			if ( !$this->executar($sql)){
				return false;
			}
			
			# Excluir Acompanhamento Relacionados
			$sql = "delete from tarefa.acompanhamento where tarid = {$id} ";
			if ( !$this->executar($sql)){
				return false;
			}
			
			# Excluir Instituições Relacionadas
			$sql = "delete from tarefa.instituicaorelacionada where tarid = {$id} ";
			if ( !$this->executar($sql)){
				return false;
			}
			
			# Excluir Documentos
			$sql = "select arqid from tarefa.anexo where tarid = {$id} ";
			$arqid = $this->pegaUm($sql);
			
			if($arqid){
				$sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid = {$arqid}";
				if ( !$this->executar($sql)){
					return false;
				}
				$sql = "delete from tarefa.anexo where tarid = {$id} ";
				if ( !$this->executar($sql)){
					return false;
				}
			}

			# Excluir Restricoes
			$sql = "delete from tarefa.restricao where tarid = {$id} ";
			if ( !$this->executar($sql)){
				return false;
			}
			return true;
			//die;
		}
		/*if($id){
			# Excluir Solicitantes Relacionados
			$sql = "delete from tarefa.solicitante where tarid = {$id} ";
			if ( !$this->executar($sql)){
				return false;
			}
			
			# Excluir Acompanhamento Relacionados
			$sql = "delete from tarefa.acompanhamento where tarid = {$id} ";
			if ( !$this->executar($sql)){
				return false;
			}
			
			# Excluir Instituições Relacionadas
			$sql = "delete from tarefa.instituicaorelacionada where tarid = {$id} ";
			if ( !$this->executar($sql)){
				return false;
			}
			
			# Excluir Documentos
			$sql = "select arqid from tarefa.anexo where tarid = {$id} ";
			$arqid = $this->pegaUm($sql);
			
			if($arqid){
				$sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid = {$arqid}";
				if ( !$this->executar($sql)){
					return false;
				}
				$sql = "delete from tarefa.anexo where tarid = {$id} ";
				if ( !$this->executar($sql)){
					return false;
				}
			}

			# Excluir Restricoes
			$sql = "delete from tarefa.restricao where tarid = {$id} ";
			if ( !$this->executar($sql)){
				return false;
			}
			
			$arDados = $this->carregar("select tarid from tarefa.tarefa where _tarpai = {$id} ");
			if($arDados){
				foreach($arDados as $dados){
					$this->excluir($dados["tarid"]);
				}
			}
			return true;			
		} else {
			echo "<script>
					alert('Ocorreu algum erro');
					history.back(-1);
				  </script>";
			die;
		}*/
	}
	
	public function recuperaInstituicoesPorTarid($tarid){
		$instituicoes = "";
		$arDadosTemp = array();
		$arDados = $this->carregar("select iesid from tarefa.instituicaorelacionada where tarid = {$tarid} ");
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
		$arDados = $this->carregar("select solnome from tarefa.solicitante where tarid = {$tarid} ");
		$arDados = ($arDados) ? $arDados : array();
		return $arDados;
	}
	
	public function recuperaNomeSetorRepon($unaid){
		return $this->pegaUm("select unadescricao from tarefa.unidade where unaid = {$unaid} ");
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
		return $this->pegaUm("select sitdsc from tarefa.situacaotarefa where sitid = '{$sitid}' ");
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
				$acodscTemp = "<b>Dados Alterados</b><br />".$acodscTemp;
				$quebra = "<br />";
			}
		}
		$acodscTemp .= "$quebra<b>Mensagem</b><br />".$acodsc;
		$sql = "INSERT INTO tarefa.acompanhamento (
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
			$acodscTemp .= "$quebra<b>Mensagem</b><br />".iconv( "UTF-8", "ISO-8859-1", $acodsc);
		}
		
		$sql = "INSERT INTO tarefa.acompanhamento (
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
	
	public function pegaTartarefaPorTarid($tarid){
		if($tarid)
			return $this->pegaUm("select _tartarefa from tarefa.tarefa where tarid = $tarid ");
	}
	
	public function boAtividade(){
		return $this->pegaUm("select _tarpai from tarefa.tarefa where tarid = $this->tarid ");
	}
	
	public function recuperaAcompanhamentoTarid($tarid){
		$sql = "";
		if($tarid){
			$sql = "SELECT 
					 to_char(a.acodata, 'DD/MM/YYYY HH24:MI:SS') AS data,
					 u.usunome,
					 a.acodsc
					FROM
					 tarefa.acompanhamento AS a 
					 LEFT JOIN seguranca.usuario AS u ON u.usucpf = a.usucpf
				    WHERE
				     a.tarid = '{$tarid}' AND 
				     a.acostatus = 'A' 
				    ORDER BY
				     a.acoid DESC";
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
				from tarefa.tarefa where tarid = $tarid";
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
}