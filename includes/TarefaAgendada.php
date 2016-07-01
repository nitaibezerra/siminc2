<?php

class TarefaAgendada
{
	
	const DIR_SCRIPT = '/var/tmp/......';
	
	protected $hora = 0;
	
	protected $minuto = 0;
	
	protected $dia = 1;
	
	protected $mes = 1;
	
	protected $script = '';
	
	protected $periodicidade = array();
	
	protected $criacao = 0;
	
	protected $ultimaexecucao = 0;
	
	public function timestamp()
	{
		if ( $this->script == '' )
		{
			return null;
		}
		$hora = (integer) $this->hora;
		$minuto = (integer) $this->minuto;
		$mes = (integer) $this->mes;
		$dia = (integer) $this->dia;
		return mktime( $hora, $minuto, 0, $mes, $dia, (integer) date( 'Y' ) );
	}
	
	protected function podeExecutar( $timestamp )
	{
		$timestamp = mktime();
		$diferenca_minuto = $timestamp - $this->ultimaexecucao;
		switch ( $this->periodicidade )
		{
			case 'minuto':
				return $diferenca_minuto >= $this->periodicidade;
			case 'hora':
				// verifica se execução ainda não ocorreu
				if ( $this->ultimaexecucao == 0 )
				{
					// verifica se horario de criacao é anterior ao de execução
					$criacao =
						( date( 'G', $this->criacao ) * 60 ) +
						date( 's', $this->criacao );
					$atual =
						( date( 'G', $timestamp ) * 60 ) +
						date( 's', $timestamp );
					return $criacao < $atual;
				}
				$diferenca_hora = $diferenca_minuto / 60;
				return $diferenca_hora >= $this->periodicidade;
			case 'dia':
				return ( $diferenca / ( 24 * 60 ) ) > $this->periodicidade;
			case 'semana':
				return ( $diferenca / ( 7 * 24 * 60 ) ) > $this->periodicidade;
			case 'semana':
				return ( $diferenca / ( 31 * 24 * 60 ) ) > $this->periodicidade;
		}
	}
	
	protected function resetSchedule()
	{
		$this->hora = 0;
		$this->minuto = 0;
		$this->dia = 1;
		$this->mes = 1;
		$this->periodicidade = '';
	}
	
	public function schedule( $hour, $minute, $day, $month )
	{
		$this->resetSchedule();
		$this->hora = (integer) $hour;
		$this->minuto = (integer) $minute;
		$this->dia = (integer) $day;
		$this->mes = (integer) $month;
	}
	
	public function scheduleMinute( $periodicidade  )
	{
		$this->resetSchedule();
		$this->periodicidade = 'minuto';
		$this->minuto = (integer) $periodicidade;
		
	}
	
	public function scheduleHour( $periodicidade, $minuto )
	{
		$this->resetSchedule();
		$this->periodicidade = 'hora';
		$this->hora = (integer) $periodicidade;
		$this->minuto = (integer) $minuto;
	}
	
	public function setScript( $arquivo )
	{
		$caminho = self::DIR_SCRIPT . basename( $arquivo );
		if ( file_exists( $caminho ) == false )
		{
			return false;
		}
		$this->script = $caminho;
		return true;
	}
}

?>