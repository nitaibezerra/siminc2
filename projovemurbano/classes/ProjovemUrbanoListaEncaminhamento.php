<?php
class ProjovemUrbanoListaEncaminhamento {

	private $perfis;
	private $infoPolo;
	private $possuiPolo = false;
	private $tituloPagina;
	private $formulario;
	private $registros;
	private $mensagem = array('diario'=>
								array( 	'pendencia' => 'Pendência de fechamento',
										'reabrir' 	=> 'Reabrir Turma(s)',
										'historico' => 'Histórico Tramitação'
								)
						);
	//Monta Workflow
	public function montaWorkflow(){
		
	}
	
	// 
	public function inicioPerfilEquipeMec( $var )
	{
		if( $this->arrayEstaVazio( $var ) ){
			throw new Exception( 'Erro ao carregar Perfil MEC.' );
		}
	}
	
	public function inicioPerfilCoordenador( $var )
	{
		if( $this->arrayEstaVazio( $var ) ){
			throw new Exception( 'Erro ao carregar Coordenador.' );
		}
	}
	
	public function inicioPerfilDiretorPolo( $var )
	{
		if( $this->arrayEstaVazio( $var ) ){
			throw new Exception( 'Erro ao carregar Diretor de Polo.' );
		}
	}
	
	public function inicioPerfilDiretorNucleo( $var )
	{
		if( $this->arrayEstaVazio( $var ) ){
			throw new Exception( 'Erro ao carregar Diretor de Núcleo.' );
		}
	}
	
	/* Recebe os registros do banco de dados */
	public function setRegistros( $var )
	{
		if( $this->arrayEstaVazio( $var ) ){
			throw new Exception( 'Nenhum registro encontrado.' );
		}else{
			$this->registros = $var;
		}
	}
	
	public function getRegistros()
	{
		return $this->registros;
	}

	/* Informações do Polo */
	public function setInfoPolo( $var )
	{
		$this->infoPolo = $var;
	}
	
	public function getInfoPolo()
	{
		return $this->infoPolo;
	}
	
	/* Possui Polo */
	public function getPossuiPolo()
	{
		$info = $this->getInfoPolo();
		
		if( !empty($info) && $info['pmupossuipolo'] == 't' ){
			$this->possuiPolo = true;
		}

		return $this->possuiPolo;
	}

	/* Variáveis de Formulário */
	public function setFormulario( $var )
	{
		if( !empty($this->formulario) ){
			array_push( $var, $this->formulario );
		}else{
			$this->formulario = $var;
		}
	}
	
	public function getFormulario()
	{
		if( empty( $this->formulario ) ){
			throw new Exception( 'Erro ao recuperar dados do formulário.' );
		}else{
			return $this->formulario;
		}
	}	
	
	/* Título Página */
	public function getTituloPagina()
	{
		if( empty( $this->tituloPagina ) ){
			throw new Exception( 'Erro ao recuperar título da página.' );
		}else{
			return $this->tituloPagina;
		}
	}
	
	public function setTituloPagina( $var )
	{
		if( empty( $var) ){
			throw new Exception( 'Erro ao definir título da página.' );
		}else{
			$this->tituloPagina = $var;
		}
	}

	/* Perfil */
	public function setPerfis( $var )
	{
		if( empty( $var) ){
			throw new Exception( 'Erro ao carregar perfis.' );
		}else{
			$this->perfis = $var;
		}
	}
	
	public function getPerfis( )
	{
		if( empty( $this->perfis ) ){
			throw new Exception( 'Erro ao recuperar título da página.' );
		}else{
			return $this->perfis;
		}
	}

	/* Funções de Apoio */

	// Tratamento de variáveis
	public function getArrayDadosFormulario()
	{
		$parametros = array();
	
		if( ( !empty($this->formulario['estuf'])
				&& $this->formulario['estuf_campo_flag'] == 1)
				|| !empty($this->formulario['estuf'][0]) ){
	
			$parametros['estuf'] = implode("','", $this->formulario['estuf']);
		}
	
		if( !empty($this->formulario['polid'])
				&& $this->formulario['polid_campo_flag'] == 1
				|| !empty($this->formulario['polid'][0]) ) {
			$parametros['polid'] = implode(',', $this->formulario['polid']);
		}
	
		if( !empty($this->formulario['nucid'])
				&& $this->formulario['nucid_campo_flag'] == 1
				|| !empty($this->formulario['nucid'][0]) ) {
			$parametros['nucid'] = implode(',', $this->formulario['nucid']);
		}
	
		if( !empty($this->formulario['estudantesaptos']) ) {
			$parametros['estudantesaptos'] = $this->formulario['estudantesaptos'];
		}
	
		if( !empty($this->formulario['estudantesinaptos']) ) {
			$parametros['estudantesinaptos'] = $this->formulario['estudantesinaptos'];
		}
	
		if( !empty($this->formulario['mundescricao']) ) {
			$parametros['mundescricao'] = $this->formulario['mundescricao'];
		}
	
		if( !empty($this->formulario['naopagamento']) ) {
			$parametros['naopagamento'] = $this->formulario['naopagamento'];
		}
	
		if( !empty($this->formulario['simpagamento']) ) {
			$parametros['simpagamento'] = $this->formulario['simpagamento'];
		}
	
		if( !empty($this->formulario['esfera']) ) {
			$parametros['esfera'] = $this->formulario['esfera'];
		}
	
		if( !empty($this->formulario['esdid']) ) {
			$parametros['esdid'] = $this->formulario['esdid'];
		}
	
		if( !empty($this->formulario['perid']) ) {
			$parametros['perid'] = $this->formulario['perid'];
		}
	
		if( !empty($this->formulario['pjuid']) ) {
			$parametros['pjuid'] = $this->formulario['pjuid'];
		}
	
		if( !empty($this->formulario['usucpf']) ) {
			$parametros['usucpf'] = $this->formulario['usucpf'];
		}
	
		if( !empty($this->formulario['possuipolo']) ) {
			$parametros['possuipolo'] = $this->formulario['possuipolo'];
		}
	
		return $parametros;
	}
	
	// Verifica se o parâmetro é um ARRAY e se seus elementos não estão vazios 
	public function arrayEstaVazio( $var )
	{
		$retorno  = true;
		$contador = 0;

		if( !empty($var) && is_array($var) && count($var) > 0 )
		{
			//throw new Exception( 'Parâmetro não é do tipo array.' );
			foreach( $var as $chave=>$valor ){

				if( empty( $var[$chave] ) ){
					$contador++;
				}
			}
			
			// Valida se foi encontrado registro vazio
			if( $contador > 0 ){
				$retorno = true;
			}else{
				$retorno = false;
			}
		}

		return $retorno;
	}
	
	// Função de Debug
	public function db( $var )
	{
		echo '<br>';
		var_dump( $var );
		exit;	
	}
}?>